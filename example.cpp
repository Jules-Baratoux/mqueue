// -*- compile-command: "g++ example.cpp mqueue.cpp exception.cpp -lrt" -*-
#include "mqueue.hh"

#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cerrno>

#define PARENT	const pid_t pid = fork(); string ID = "C"; if (pid) { ID = "P";
#define CHILD	waitpid(pid, NULL, 0); } else

#define print(VAR) std::cout << #VAR" → " << VAR << std::endl;

#include <cstring>

void		clear(void)
{
	struct mq::attr	attr;
	const string	name("/a");

	PARENT
	{
		char            buffer[attr.msgsize];
		unsigned int    priority;
		struct mq       mq;

		mq.open(name, O_CREAT | O_RDONLY, S_IRUSR | S_IWUSR, attr);
		mq.receive(buffer, attr.msgsize, &priority);
		mq.close();
		mq.unlink();

		print(buffer); print(priority);
	}

	CHILD
	{
		struct mq       mq;

		mq.open(name, O_WRONLY);
		mq.send("clear ", 3);
		mq.close();
	}
}

void		simple(void)
{
	struct mq::attr	attr;
	const string	name("/a");

	PARENT
	{
		char            buffer[attr.msgsize];
		unsigned int    priority;

		struct mq       mq("", O_CREAT | O_RDONLY, S_IRUSR | S_IWUSR, attr);

		mq.receive(buffer, attr.msgsize, &priority);

		print(buffer); print(priority);
	}

	CHILD
	{
		struct mq       mq(name, O_WRONLY);

		mq.send(string("simple"), 0);
		exit(EXIT_SUCCESS);
	}
}

struct handle
{
	static const size_t sizeof_name = 32;

	int	pid;
	char	name[sizeof_name];

	handle(void)
	{ }

	handle(int pid, const string& name) : pid(pid)
	{
		::strncpy(this->name, name.c_str(), sizeof_name);
	}
};


#include <mqueue.h>

int		main(int argc, const char *argv[])
{
	const string	name(argv[1] ?: "/a");
	struct mq::attr	attr;
	int		fd;

	print(name);
	attr.flags = O_NONBLOCK;
	attr.msgsize = sizeof(struct handle);

	PARENT
	{
		struct mq       mq(name, O_CREAT | O_RDONLY, S_IRUSR | S_IWUSR, attr);
		struct handle	dst;

		mq.receive(dst);

		print(dst.pid);
		print(dst.name);
		mq.unlink();
	}

	CHILD
	{
		struct mq       mq(name, O_WRONLY);
		struct handle	src(42, "ça marche");

		mq.send(src, 5);
		write(mq, "", 0);

		exit(EXIT_SUCCESS);
	}
	return EXIT_SUCCESS;
}
