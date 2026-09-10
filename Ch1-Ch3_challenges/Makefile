#This is for Java configuration
# Variables
JC = javac
JVM = java
MAIN = HelloWorld

#run
run-java: compile-java
		$(JVM) $(MAIN_JAVA)

#compilation Rule
compile-java:
		$(JC) $(MAIN).java

# This is for C configuration 
#variables
CC = gcc
CFlags = -Wall -g
MAIN_C = HelloWorld


#Execution Rule
run-c: compile-c
		./$(MAIN_C)

compile-c: 
		$(CC) $(FLAGS) -o $(MAIN_C) $(MAIN_C).c

# Global rules
# when using the 'make' command it will clean up and run both languages back to abck

all: clean compile-java compile-c
		@echo "-- RUNNING JAVA PROGRAM ---"
		$(JvM) $(MAIN_JAVA)
		@echo "\n-- RUNNING C PROGRAM ---"
		./$(MAIN_C)
clean: 
		rm -f *.class $(MAIN)