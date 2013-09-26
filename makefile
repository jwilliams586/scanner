CC=gcc

testScanner: driveScanner.o scanner.o 
	$(CC) -o testScanner driveScanner.o scanner.o
