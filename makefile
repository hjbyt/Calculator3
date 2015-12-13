all: 
	cd SP; make all
	$(MAKE) ex3
ex3: 

clean:
	cd SP; make clean
	#Invoke a remove command to clean up the current Directory (NOTE: The directory SP is already Clean)
