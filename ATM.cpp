//
//  ATM.cpp
//  
//
//  Created by Sagar Chowdhury on 8/27/20.
//
//

#include <stdio.h>
#include <iostream>
#include <unordered_map>
using namespace std;

#define maxTry 4 //Maximum trials the user can attempt


//This Account class keeps track of balance, withdrawal, deposit.
class Account{
private:
    int balance;

public:
    Account(){balance =0;}
    Account(int bl):
        balance(bl)
        {}
    int seeBalance(){return balance;}
    bool withdrawBalance(int amount){
        if (amount<balance) {
            balance -=amount;
//            cout<<"balance:"<<balance<<"\n";
            return true;
        }
        return false;
        
    }
    bool depositBalance(int amount){
//        cout<<"balance before:"<<balance<<"\n";
        balance +=amount;
//        cout<<"balance after:"<<balance<<"\n";
        return true;
    }
    
};

//AccountHolder class keeps track of account owner and all his/her accounts. An account holder can have multiple accounts that can be accessed with a single ATM card.
class AccountHolder{
private:
    int pin;
    string name;
    
public:
    unordered_map<long long, Account> individualAccounts;
    AccountHolder(){}
    void addPin(int iPin){pin = iPin;}
    void addName(string iName){cout<<"name:"<<iName<<"\n";name = iName;}
    void addAccount(long long iAccNum, int iDeposit){
        Account ac(iDeposit);
        individualAccounts[iAccNum]=ac;
    }
    bool makeDeposit(long long iAccNum, int iDeposit){
        return individualAccounts[iAccNum].depositBalance(iDeposit);
    }
    int checkBalance(long long iAccNum){
        return individualAccounts[iAccNum].seeBalance();
    }
    bool makeWithdrawal(long long iAccNum, int iWithdrawal){
        return individualAccounts[iAccNum].withdrawBalance(iWithdrawal);
    }
    bool isValid(int enteredPin){
        return pin == enteredPin;
    }
    
    
};


//AllAccounts maps all the account holders. Each account holder has unique ATM card. All the accounts of the account holder is linked with his/her ATM card.
unordered_map<long long, AccountHolder>AllAccounts;


//createNewAccount function is created to make new test cases. Ideally all the accounts information should come from some databases.
//createNewAccount assign a new ATM card if it is a new customer. Otherwise, it links all the new accounts to the existing ATM card.
//It does not allow to create a new account with an existing account number of the account holder. However, multiple account holders can have same account number since they can be distinguished by the atm card number.
bool createNewAccount(){
    long long atmNum;
    long long accountNum;
    int pin;
    int deposit;

    char name[256];
    printf("Enter atm card number:\n");
    scanf("%lld", &atmNum);
    if (AllAccounts.count(atmNum)==0) {
        AccountHolder acHolder;
        AllAccounts[atmNum] = acHolder;
        printf("Enter a pin\n");
        scanf("%d", &pin);
        AllAccounts[atmNum].addPin(pin);
        printf("Enter a name\n");
        scanf("%s", name);
        AllAccounts[atmNum].addName(name);
        
        
    }
    
    int trial =0;
    while (true) {
        printf("Enter accountNumber\n");
        scanf("%lld", &accountNum);
        if (AllAccounts[atmNum].individualAccounts.count(accountNum)>0) {
            if(trial>=maxTry){printf("maximum failed tries exceeds.please start over.\n");return false;}
            printf("this account already exists. choose a different account number\n");
            trial++;
        }
        else{
            printf("Enter deposit\n");
            scanf("%d", &deposit);
            AllAccounts[atmNum].addAccount(accountNum, deposit);
            return true;
            
        }
        
        
    }
    

    
}

//Verify whether the inserted card number is existed in the AllAccounts. Otherwise, it fails to proceed.
bool verifyCard(long long& atmNum){
    long long atmCardNo;
    printf("Welcome to the bank\n");
    printf("Please insert your card\n"); //For now we will enter the card number here.
    scanf("%lld", &atmCardNo);
    if (AllAccounts.count(atmCardNo)>0) {
        atmNum = atmCardNo;
        return true;
    }
    else{
        printf("cannot find the account. Remove the card and try again\n");
        return false;
    }
    
}

//Verify whether the user entered correct pin. It gives the user a maximum number of attempts (maxTry) to enter correct pin. If it fails to enter correct pin, the user has to start over. This function is simplified. It does not block the user to access the account after maxTry attempts.

bool verifyAccount(long long& atmNum){
    int numTry = 0;
    int pinNumber;
    while (numTry<maxTry) {
        printf("Please enter your pin\n");
        scanf("%d", &pinNumber);
        if (AllAccounts[atmNum].isValid(pinNumber)) {
            printf("Account is successully accessed\n");
            return true;
            
        }
        else{
            if (numTry==maxTry) {
                printf("is not valid pin. maximum trials exceeded. you cannont access the account.\n");
                return false;
            }
            else{
                printf("is not a valid pin, please try again\n");
                
            }
            numTry++;
            
        }
    }
    return true;
}

//Let the user to choose the account he/she wants to access.

bool chooseAccount(long long& atmNum, long long& actNum){
    int doYouWantToAccessThisAccount;
    
    for (
         unordered_map<long long, Account>::iterator it = AllAccounts[atmNum].individualAccounts.begin() ; it !=AllAccounts[atmNum].individualAccounts.end(); ++it) {
        printf("do you want to access the account: %lld, 0->no, 1->yes\n", it->first);
        scanf("%d", &doYouWantToAccessThisAccount);
        if (doYouWantToAccessThisAccount) {
            actNum = it->first;
            return true;
        }
      
    }
    return false;
    
}

//This function does all the transactions. It takes the user input what type of transactions he/she wants (checkBalance, deposit, withdrawal). It does not let the user to withdraw money if the requested amount falls below the balance. It gives a maxtry attempt to correct the withdrawal amount. If the user fails to enter the acceptable amount, he/she has to start over.

void transaction(long long& atmNum, long long& actNum){
    
    while (true) {
        int choice;
        printf("Do you want to see the balance. then enter 0\n");
        printf("Do you want to deposit. then enter 1\n");
        printf("Do you want to withdraw. then enter 2\n");
        scanf("%d", &choice);

        switch (choice) {
            case 0:
                printf("your balance is: %d\n",AllAccounts[atmNum].checkBalance(actNum));
                break;
            case 1:
            {
                int amnt;
                printf("please enter the amount\n");
                scanf("%d", &amnt);
                AllAccounts[atmNum].makeDeposit(actNum, amnt);
                printf("deposit successful\n");
                break;
            }
            case 2:
            {
                int trial = 0;
                while (true) {
                    int amntWithdraw;
                    printf("please enter the amount\n");
                    scanf("%d", &amntWithdraw);
                    if (!AllAccounts[atmNum].makeWithdrawal(actNum, amntWithdraw)) {
                        if (trial>=maxTry) {
                            printf("too many failed trials. please start over\n");
                            break;
                        }
                        printf("you dont have enough balance for this transaction, please choose another amount\n");
                        trial++;
                    }
                    else{
                        printf("successful transaction\n");
                        break;
                    }
                    
                }
            }
            default:
                break;
                
        }
        int moreTrans;
        
        printf("Do you want to make more transactions on this account? 0->no, 1->yes\n");
        scanf("%d", &moreTrans);
        if(!moreTrans){
            break;
        }
        
    }
    
}



int main(){
    //testCase creation:
    
    //call createNewAccount function to create as many accounts as needed for testing.
    
    
    //createAccounts:
    //1)Needs a new/existing atm card.
    //2)Needs new account number.
    //3)Needs deposit.
    //4)Needs a new/existing pin number.
    if(createNewAccount()){printf("successfully created a new account\n");}
    else{
      printf("failed to  create a new account\n");
    }
    if(createNewAccount()){printf("successfully created a new account\n");}
    else{
        printf("failed to  create a new account\n");
    }
    if(createNewAccount()){printf("successfully created a new account\n");}
    else{
        printf("failed to  create a new account\n");
        
    }
    
    
    
    //Lets start transactions:
    //1) verify the atm card
    //2) verify the bank account with pin number
    //3) choose the account the user wants to access
    //4) choose the type of transactions
    //5) do the transaction
    //6) do you want to have more transactions
    //7) thank you.
    
    //
    
    //ATM transactions
    
    //verify the atm card
    long long atmCardNumber;
    if(!verifyCard(atmCardNumber)){
        printf("card cannot be verified. Please enter a valid card\n");
    }
    else{
        //verify account
        long long accNum;
        if (!verifyAccount(atmCardNumber)) {
            printf("Your account cannot be verfied. Please talk to a customer support\n");
        }
        else{
            while (true) {
                //choose an account for transaction
                if(chooseAccount(atmCardNumber, accNum)){
                    //make transaction
                    transaction(atmCardNumber, accNum);
                    int transfromAnotherAcc;
                    printf("Do you want to make more transactions: 0->no, 1->yes\n");
                    scanf("%d", &transfromAnotherAcc);
                    if(!transfromAnotherAcc)break;
                    
                }
                else{
                    int tryAgain;
                    printf("You did not choose an account. Do you want try again: 0->no, 1->yes\n");
                    scanf("%d", &tryAgain);
                    if (!tryAgain) {
                        break;
                    }
                    
                }
            }
            
        }
    }
    printf("Thank you. Have a wonderful day.\n");
    
    
    
    return 0;
}
