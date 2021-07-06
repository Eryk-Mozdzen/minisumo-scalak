#ifndef UART_H
#define UART_H

class Stream {
	public:
		Stream();
		void begin(unsigned long);
		void print(char*);
		void print(int);
		void println(char*);
		void println(int);
		bool listen();
		char* getString();
		
		static char* intToString(int);
		static int stringToInt(char*);
		
		static char* getCommandType(char*);
		static int getCommandIndex(char*);
		static int getCommandValue(char*);
		static char* createCommand(char*, int, int);
	private:
		char buffer[64];
		char str[64];
		int bufferLen;
		
		void write(unsigned char);
		unsigned char read();
};

bool equal(char*, char*);

#endif