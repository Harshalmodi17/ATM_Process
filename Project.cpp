#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class ATM
{
private:
    int accountNumber = 1000;
    bool isLoggedIn = false;
    string currentAccountNumber;

public:
    void displayMenu()
    {
        cout << "ATM Menu:\n";
        cout << "1. Create Account\n";

        if (isLoggedIn)
        {
            cout << "2. Deposit\n";
            cout << "3. Withdraw\n";
            cout << "4. Logout\n";
        }
        else
        {
            cout << "2. Login\n";
        }

        cout << "5. Exit\n";
    }

    void Create_Account()
    {
        string name, pin;
        double initial_deposit;
        int acNum;
        acNum = accountNumber;
        ifstream acc("C:/C++/Account_number.txt", ios_base::in);
        int linecount = 0;
        string line;
        while (getline(acc, line))
        {
            linecount++;
        }
        acc.close();
        cin.ignore();
        cout << "Thank You for choosing our bank." << endl;
        cout << "Please enter your name: ";
        getline(cin, name);
        cout << "Please enter your initial deposit amount: ";
        cin >> initial_deposit;
        cout << "Create your pin: ";
        cin >> pin;
        cout << "Generating your account number ..." << endl;
        cout << "Your Account number is: " << acNum + linecount << endl;

        ofstream pi("C:/C++/Pin.txt", ios_base::app);
        pi << pin << "\n";
        pi.close();

        ofstream nam("C:/C++/Name.txt", ios_base::app);
        nam << name << "\n";
        nam.close();

        ofstream accin("C:/C++/Account_number.txt", ios_base::app);
        accin << acNum + linecount << "\n";
        accin.close();

        ofstream depo("C:/C++/Deposit.txt", ios_base::app);
        depo << initial_deposit << "\n";
        depo.close();

        cout << "Your account is successfully created" << endl;
        cout << "Thank you for choosing our Bank" << endl;
    }

    int Login()
    {
        string enteredAccountNumber, enteredPin;
        cout << "Please enter your account number: ";
        cin >> enteredAccountNumber;

        ifstream accountNumbersFile("C:/C++/Account_number.txt", ios_base::in);
        ifstream pinsFile("C:/C++/Pin.txt", ios_base::in);

        int lineNumber = 1; // Initialize the line number

        bool found = false;
        string storedAccountNumber, storedPin;

        while (accountNumbersFile >> storedAccountNumber && pinsFile >> storedPin)
        {
            if (storedAccountNumber == enteredAccountNumber)
            {
                found = true;
                cout << "Please enter your PIN: ";
                cin >> enteredPin;
                if (storedPin == enteredPin)
                {
                    cout << "Login successful. Welcome, " << storedAccountNumber << "!\n";

                    currentAccountNumber = enteredAccountNumber; // Set the current account number
                    postLoginMenu();                             // Call the post-login menu
                    isLoggedIn = true;                           // Set the login status to true
                    accountNumbersFile.close();
                    pinsFile.close();
                    return lineNumber;
                }
                else
                {
                    cout << "Invalid PIN. Login failed.\n";
                    accountNumbersFile.close();
                    pinsFile.close();
                    return -1; // Add a return value for the failure case
                }
            }

            lineNumber++; // Increment the line number
        }

        if (!found)
        {
            cout << "Account not found. Please check your account number.\n";
        }

        accountNumbersFile.close();
        pinsFile.close();
        return -1; // Add a return value for the case where the account is not found
    }

    void withdraw()
    {
        double enteredAmount;
        cout << "Enter the withdrawal amount: "; // Prompt for withdrawal amount
        cin >> enteredAmount;

        ifstream wid("C:/C++/Deposit.txt");
        ofstream tempFile("C:/C++/Deposit_temp.txt");

        int currentLineNumber = 1;
        double storedDeposit;
        bool accountFound = false;

        while (wid >> storedDeposit)
        {
            if (currentLineNumber == getAccountLine())
            {
                accountFound = true;
                if (storedDeposit < enteredAmount)
                {
                    cout << "Insufficient balance. Withdrawal failed.\n";
                    tempFile << storedDeposit << "\n"; // Write back the original balance
                }
                else
                {
                    double newBalance = storedDeposit - enteredAmount;
                    tempFile << newBalance << "\n"; // Write the updated balance
                    cout << "Withdrawal successful. Your new balance is: " << newBalance << endl;
                }
            }
            else
            {
                tempFile << storedDeposit << "\n"; // Write back other balances
            }

            currentLineNumber++;
        }

        wid.close();
        tempFile.close();

        // Remove the original Deposit.txt file and rename the temp file
        if (accountFound)
        {
            remove("C:/C++/Deposit.txt");
            rename("C:/C++/Deposit_temp.txt", "C:/C++/Deposit.txt");
        }
        else
        {
            cout << "Account not found. Withdrawal failed.\n";
            remove("C:/C++/Deposit_temp.txt"); // Remove the temporary file
        }
    }

    void deposit()
    {
        double enteredAmount;
        cout << "Enter the deposit amount: "; // Prompt for deposit amount
        cin >> enteredAmount;

        ifstream depositFile("C:/C++/Deposit.txt");
        ofstream tempFile("C:/C++/Deposit_temp.txt");

        int currentLineNumber = 1;
        double storedDeposit;
        bool accountFound = false;

        while (depositFile >> storedDeposit)
        {
            if (currentLineNumber == getAccountLine())
            {
                accountFound = true;
                double newBalance = storedDeposit + enteredAmount;
                tempFile << newBalance << "\n"; // Write the updated balance
                cout << "Deposit successful. Your new balance is: " << newBalance << endl;
            }
            else
            {
                tempFile << storedDeposit << "\n"; // Write back other balances
            }

            currentLineNumber++;
        }

        depositFile.close();
        tempFile.close();

        // Remove the original Deposit.txt file and rename the temp file
        if (accountFound)
        {
            remove("C:/C++/Deposit.txt");
            rename("C:/C++/Deposit_temp.txt", "C:/C++/Deposit.txt");
        }
        else
        {
            cout << "Account not found. Deposit failed.\n";
            remove("C:/C++/Deposit_temp.txt"); // Remove the temporary file
        }
    }

    void postLoginMenu()
    {
        int choice;
        while (true)
        {
            displayMenu(); // Show the ATM menu
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice)
            {
            case 2:
                deposit();
                break;
            case 3:
                withdraw();
                break;
            case 4:
                isLoggedIn = false; // Set isLoggedIn to false upon logout.
                cout << "Logout successful. Goodbye!\n";
                displayMenu();
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
            }
        }
    }

    int getAccountLine()
    {
        // Returns the line number for the currently logged-in account
        string enteredAccountNumber = currentAccountNumber;

        ifstream accountNumbersFile("C:/C++/Account_number.txt", ios_base::in);

        int lineNumber = 1; // Initialize the line number
        string storedAccountNumber;

        while (accountNumbersFile >> storedAccountNumber)
        {
            if (storedAccountNumber == enteredAccountNumber)
            {
                accountNumbersFile.close();
                return lineNumber;
            }

            lineNumber++; // Increment the line number
        }

        accountNumbersFile.close();
        return -1; // Account not found
    }

    bool IsLoggedIn()
    {
        return isLoggedIn;
    }
};

int main()
{
    ATM a;
    int choice;

    while (true)
    {
        a.displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            a.Create_Account();
            break;
        case 2:
            if (!a.IsLoggedIn())
            {
                a.Login();
            }
            else
            {
                cout << "You are already logged in.\n";
            }
            break;
        case 5:
            cout << "Exiting ATM. Goodbye!\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
