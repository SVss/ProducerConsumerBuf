#include "stdafx.h"

using namespace std;

static queue<string> buffer;		// реализация с бесконечным буферром на очереди
static mutex bufferMutex;
static bool isEndOfInput = false;

void produce(ifstream *input)
{
	isEndOfInput = false;

	string str = "";
	while (!input->eof())
	{
		getline( (*input), str);

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

int main(int argc, _TCHAR* argv[])
{
	if (argc < 3) {
		cerr << "Not enough arguments" << endl;
		return -1;
	}

	ifstream inputFile(argv[1]);
	if(!inputFile.good() )
	{
		cerr << "Can't open input file"  << endl;
		return -1;
	}

	ofstream outputFile(argv[2]);
	if(!outputFile.good() )
	{
		cerr << "Can't open output file"  << endl;
		return -1;
	}

	thread prod(produce, &inputFile);
	thread cons(consume, &outputFile);

	prod.join();
	cons.join();

	inputFile.close();
	outputFile.close();

	return 0;
}
