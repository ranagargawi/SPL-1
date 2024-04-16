all: warehouse
warehouse: Action Customer main Order Volunteer WareHouse
	g++ -g -Wall -std=c++11 -Iinclude -Weffc++ -o bin/WareHouse bin/Action.o bin/Customer.o bin/main.o bin/Order.o bin/Volunteer.o bin/WareHouse.o
Action:
	g++ -c -g -Wall -std=c++11 -Iinclude -Weffc++ -o bin/Action.o src/Action.cpp
Customer:
	g++ -c -g -Wall -std=c++11 -Iinclude -Weffc++ -o bin/Customer.o src/Customer.cpp
main:
	g++ -c -g -Wall -std=c++11 -Iinclude -Weffc++ -o bin/main.o src/main.cpp
Order:
	g++ -c -g -Wall -std=c++11 -Iinclude -Weffc++ -o bin/Order.o src/Order.cpp
Volunteer:
	g++ -c -g -Wall -std=c++11 -Iinclude -Weffc++ -o bin/Volunteer.o src/Volunteer.cpp
WareHouse:
	g++ -c -g -Wall -std=c++11 -Iinclude -Weffc++ -o bin/WareHouse.o src/WareHouse.cpp

clean:
	rm -f bin/*.o bin/WareHouse