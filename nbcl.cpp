/************************************
** No Bullsh*t Command Line Parser **
** nbcl.cpp                        **
** Copyright 2011 OmegaSDG         **
************************************/

#include <stdio.h>

#include "nbcl.h"

/* I'm aware that the code is ugly, but it gets the job done.
   This will be refactored in a later release.
*/

NBCL::NBCL(int argc, char** argv) :
	argc(argc), argv(argv)
{
}

NBCL::~NBCL()
{
	unsigned int opt;

	for (opt = 0; opt < OptionsList.size(); opt++)
		delete OptionsList[opt];
}

void NBCL::insert(std::string shortopt, std::string longopt,
	std::string argument, std::string description)
{
	Option* opt = new Option;
	opt->shortopt = shortopt;
	opt->longopt = longopt;
	opt->argument = argument;
	opt->description = description;
	opt->present = false;
	opt->value = "";
	OptionsList.push_back(opt);
}

bool NBCL::parse()
{
	int arg;
	unsigned int opt;
	bool found;

	for (arg = 1; arg < argc; arg++) {
		found = false;
		for (opt = 0; opt < OptionsList.size(); opt++) {
			if (!OptionsList[opt]->shortopt.compare(argv[arg]) ||
			    !OptionsList[opt]->longopt.compare(argv[arg])) {
				found = true; /* We found the option. */
				OptionsList[opt]->present = true;
				if (!OptionsList[opt]->argument.empty()) {
					if (arg + 1 < argc) {
						OptionsList[opt]->value =
						    argv[arg+1];
						arg += 1;
					}
					else
						return false;
				}
				break;
			}
		}
		if (!found) /* We didn't find the option. */
			return false;
	}
	return true;
}

bool NBCL::check(std::string longopt)
{
	unsigned int search;

	for (search = 0; search < OptionsList.size(); search++) {
		if (!OptionsList[search]->longopt.compare(longopt))
			return OptionsList[search]->present;
	}

	return false;
}

std::string NBCL::get(std::string longopt)
{
	unsigned int search;

	for (search = 0; search < OptionsList.size(); search++) {
		if (!OptionsList[search]->longopt.compare(longopt))
			return OptionsList[search]->value;
	}

	return "";
}

void NBCL::usage()
{
	int optmaxlen = 0;
	int argmaxlen = 0;

	usageSize(&optmaxlen, &argmaxlen);
	usagePrintShort();
	usagePrintLong(optmaxlen, argmaxlen);
}

void NBCL::usageSize(int* optmaxlen, int* argmaxlen)
{
	unsigned int opt;

	for (opt = 0; opt < OptionsList.size(); opt++) {
		if (OptionsList[opt]->shortopt.size() +
		    OptionsList[opt]->longopt.size() > (unsigned)*optmaxlen)
			*optmaxlen = (int)OptionsList[opt]->shortopt.size() +
			    (int)OptionsList[opt]->longopt.size() + 1;
		if ((int)OptionsList[opt]->argument.size() > *argmaxlen)
			*argmaxlen = (int)OptionsList[opt]->argument.size();
	}
}

void NBCL::usagePrintShort()
{
	fprintf(stderr, "Usage: %s [OPTIONS]", argv[0]);
}

void NBCL::usagePrintLong(int optmaxlen, int argmaxlen)
{
	unsigned int opt;

	fprintf(stderr, "\n\nAvailable options:\n");

	for (opt = 0; opt < OptionsList.size(); opt++) {
		if (!OptionsList[opt]->shortopt.empty())
			fprintf(stderr, "  %s,",
			    OptionsList[opt]->shortopt.c_str());
		else
			fprintf(stderr, "     ");
		fprintf(stderr,
		    (std::string("%-")+itostr(optmaxlen+1)+"s").c_str(),
		    OptionsList[opt]->longopt.c_str());
		if (!OptionsList[opt]->argument.empty())
			fprintf(stderr,
			    (std::string("%-")+itostr(argmaxlen+3)+"s").c_str(),
			    OptionsList[opt]->argument.c_str());
		else
			fprintf(stderr,
			    (std::string("%-")+itostr(argmaxlen+3)+"c").c_str(),
			    ' ');
		fprintf(stderr, "%s\n",
		    OptionsList[opt]->description.c_str());
	}

	fprintf(stderr, "\n\n");
}

std::string NBCL::itostr(int in)
{
	std::stringstream out;
	out << in;
	return out.str();
}

