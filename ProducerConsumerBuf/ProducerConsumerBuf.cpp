#include "stdafx.h"

using namespace std;

static queue<string> buffer;		// infinite buffer on std::queue
static mutex bufferMutex;
static bool isEndOfInput = false;

void produce(ifstream *input)
{
	isEndOfInput = false;

	string str = "";
	while (!input->eof())
	{
		getline( (*input), str);
//		cout << str << endl;

		bufferMutex.lock();
		buffer.push(str);
		bufferMutex.unlock();
	}

	isEndOfInput = true;
}

void consume(ofstream *output)
{
	string str = "";
	unsigned long i = 0;

	while (!isEndOfInput)
	{
		if (!buffer.empty())
		{
			bufferMutex.lock();

			(*output) << i << " " << buffer.front() << endl;
			buffer.pop();

			bufferMutex.unlock();
			++i;
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc < 2) {
		cerr << "Not enough arguments" << endl;
		return -1;
	}

	char *inName = argv[1];

	ifstream inputFile(inName);
	if(!inputFile.good() )
	{
		cerr << "Can't open input file"  << endl;
		return -1;
	}


	char *outName = "out.txt";
	if (argc > 2)
	{
		outName = argv[2];
	}

	ofstream outputFile(outName);
	if(!outputFile.good() )
	{
		cerr << "Can't open output file"  << endl;
		return -1;
	}


	thread prod(produce, &inputFile);
	thread cons(consume, &outputFile);


	prod.join();
	inputFile.close();

	cons.join();
	outputFile.close();

	return 0;
}
