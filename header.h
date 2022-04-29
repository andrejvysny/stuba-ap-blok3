//
// Created by vysny on 19. 4. 2022.
//


void configAndConnect();

void display();
void handle();

int sendData(char data[]);
int receiveData();

int IsPrime(int number) {
    if (number == 0 || number ==1) return 0;
    for (int i = 2; i < number; i++) {
        if (number % i == 0 && i != number) return 0;
    }
    return 1;
}