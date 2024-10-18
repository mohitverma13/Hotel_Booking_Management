#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <limits>
#include <algorithm>
using namespace std;

// Modern C++ class for Hotel Management
class Hotel
{
private:
    struct CustomerRecord
    {
        int room_no;
        string name;
        string address;
        string phone;
        int days;
        float fare;
    } record;

    static constexpr float DAILY_RATE = 500.0f;

private:
    void clear_screen()
    {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    void pause()
    {
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
    void clear_input_buffer()
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

public:
    // Moved display_header to public section since it's needed by main()
    void display_header()
    {
        clear_screen();
        cout << "===================================\n";
        cout << "         XYZ Group of Hotels       \n";
        cout << "===================================\n";
    }

    void main_menu();
    void add();
    void display();
    void rooms();
    void edit();
    bool check(int room_number);
    void modify(int room_number);
    void delete_rec(int room_number);
};

void Hotel::main_menu()
{
    int choice = 0;
    while (choice != 5)
    {
        display_header();
        cout << "\n\t\tMAIN MENU\n";
        cout << "\n1. Book A Room";
        cout << "\n2. Customer Record";
        cout << "\n3. Rooms Allotted";
        cout << "\n4. Edit Record";
        cout << "\n5. Exit";
        cout << "\n\nEnter Your Choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0;
        }

        switch (choice)
        {
        case 1:
            add();
            break;
        case 2:
            display();
            break;
        case 3:
            rooms();
            break;
        case 4:
            edit();
            break;
        case 5:
            break;
        default:
            cout << "\nInvalid choice! Please try again.";
            pause();
        }
    }
}

void Hotel::add()
{
    display_header();
    int room_number;
    std::cout << "\nEnter Customer Details\n";
    std::cout << "----------------------\n";
    std::cout << "\nRoom no: ";

    while (!(std::cin >> room_number))
    {
        std::cin.clear();
        clear_input_buffer();
        std::cout << "Invalid input. Please enter a number: ";
    }
    clear_input_buffer(); // Clear the newline after number input

    if (check(room_number))
    {
        std::cout << "\nSorry! Room is already booked";
    }
    else
    {
        record.room_no = room_number;

        std::cout << "Name: ";
        std::getline(std::cin, record.name);

        std::cout << "Address: ";
        std::getline(std::cin, record.address);

        std::cout << "Phone No: ";
        std::getline(std::cin, record.phone);

        std::cout << "No of Days to Checkout: ";
        while (!(std::cin >> record.days))
        {
            std::cin.clear();
            clear_input_buffer();
            std::cout << "Invalid input. Please enter a number: ";
        }

        record.fare = record.days * DAILY_RATE;

        std::ofstream fout("Record.dat", std::ios::binary | std::ios::app);
        fout.write(reinterpret_cast<char *>(&record), sizeof(CustomerRecord));
        fout.close();

        std::cout << "\nRoom is booked successfully!";
    }

    clear_input_buffer(); // Clear the buffer before pausing
    pause();
}

void Hotel::display()
{
    display_header();
    int room_number;
    bool found = false;

    cout << "\nEnter room number: ";
    cin >> room_number;

    ifstream fin("Record.dat", ios::binary);
    while (fin.read(reinterpret_cast<char *>(&record), sizeof(CustomerRecord)))
    {
        if (record.room_no == room_number)
        {
            display_header();
            cout << "\nCustomer Details";
            cout << "\n----------------";
            cout << "\nRoom no: " << record.room_no;
            cout << "\nName: " << record.name;
            cout << "\nAddress: " << record.address;
            cout << "\nPhone no: " << record.phone;
            cout << "\nDays: " << record.days;
            cout << "\nTotal Fare: " << record.fare;
            found = true;
            break;
        }
    }
    fin.close();

    if (!found)
    {
        cout << "\nRoom not found or vacant!";
    }
    pause();
}

void Hotel::rooms()
{
    display_header();
    ifstream fin("Record.dat", ios::binary);

    cout << "\n\tList Of Rooms Allotted";
    cout << "\n\t----------------------\n";

    while (fin.read(reinterpret_cast<char *>(&record), sizeof(CustomerRecord)))
    {
        cout << "\nRoom no: " << record.room_no;
        cout << "\nName: " << record.name;
        cout << "\nAddress: " << record.address;
        cout << "\nPhone: " << record.phone;
        cout << "\nDays: " << record.days;
        cout << "\nTotal: " << record.fare;
        cout << "\n**********************************";
    }
    fin.close();
    pause();
}

void Hotel::edit()
{
    display_header();
    int choice, room_number;

    cout << "\nEDIT MENU";
    cout << "\n---------";
    cout << "\n1. Modify Customer Record";
    cout << "\n2. Delete Customer Record";
    cout << "\nEnter your choice: ";
    cin >> choice;

    cout << "\nEnter room number: ";
    cin >> room_number;

    switch (choice)
    {
    case 1:
        modify(room_number);
        break;
    case 2:
        delete_rec(room_number);
        break;
    default:
        cout << "\nInvalid Choice!";
    }
    pause();
}

bool Hotel::check(int room_number)
{
    ifstream fin("Record.dat", ios::binary);
    bool found = false;

    while (fin.read(reinterpret_cast<char *>(&record), sizeof(CustomerRecord)))
    {
        if (record.room_no == room_number)
        {
            found = true;
            break;
        }
    }
    fin.close();
    return found;
}

void Hotel::modify(int room_number)
{
    display_header();
    bool found = false;
    fstream file("Record.dat", ios::in | ios::out | ios::binary);

    while (!file.eof() && !found)
    {
        streampos pos = file.tellg();
        file.read(reinterpret_cast<char *>(&record), sizeof(CustomerRecord));

        if (file && record.room_no == room_number)
        {
            cout << "\nEnter New Details";
            cout << "\n-----------------";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "\nName: ";
            getline(cin, record.name);

            cout << "Address: ";
            getline(cin, record.address);

            cout << "Phone no: ";
            getline(cin, record.phone);

            cout << "Days: ";
            cin >> record.days;

            record.fare = record.days * DAILY_RATE;

            file.seekp(pos);
            file.write(reinterpret_cast<char *>(&record), sizeof(CustomerRecord));
            cout << "\nRecord modified successfully!";
            found = true;
        }
    }
    file.close();

    if (!found)
    {
        cout << "\nRoom not found or vacant!";
    }
}

void Hotel::delete_rec(int room_number)
{
    display_header();
    bool found = false;
    char confirm;

    ifstream fin("Record.dat", ios::binary);
    ofstream temp("temp.dat", ios::binary);

    while (fin.read(reinterpret_cast<char *>(&record), sizeof(CustomerRecord)))
    {
        if (record.room_no == room_number)
        {
            cout << "\nName: " << record.name;
            cout << "\nAddress: " << record.address;
            cout << "\nPhone No: " << record.phone;
            cout << "\nDays: " << record.days;
            cout << "\nTotal Fare: " << record.fare;
            cout << "\n\nDelete this record? (y/n): ";
            cin >> confirm;

            if (tolower(confirm) != 'y')
            {
                temp.write(reinterpret_cast<char *>(&record), sizeof(CustomerRecord));
            }
            found = true;
        }
        else
        {
            temp.write(reinterpret_cast<char *>(&record), sizeof(CustomerRecord));
        }
    }

    fin.close();
    temp.close();

    if (found)
    {
        remove("Record.dat");
        rename("temp.dat", "Record.dat");
        cout << "\nRecord deleted successfully!";
    }
    else
    {
        remove("temp.dat");
        cout << "\nRoom not found or vacant!";
    }
}

// Simulates loading animation
void show_loading_animation()
{
    cout << "\nConnecting to Server\nSyncing Data";
    for (int i = 0; i < 10; ++i)
    {
        cout << ".";
        this_thread::sleep_for(chrono::milliseconds(200));
    }
    cout << "\n";
}

int main()
{
    Hotel hotel;
    string username, password;

    // Display intro
    cout << "\n\tC++ Project On Hotel Management System\n";
    cout << "\n\tMade by: Mohit Kumar\n";
    show_loading_animation();
    cout << "\nPress Enter to continue...";
    cin.get();

    hotel.display_header();
    cout << "\nUsername: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    show_loading_animation();

    if (username == "admin" && password == "12345678")
    {
        cout << "\nLogin Successful!\n";
        this_thread::sleep_for(chrono::seconds(1));
        hotel.main_menu();
    }
    else
    {
        cout << "\nInvalid Credentials!\n";
    }

    return 0;
}