#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <conio.h>
#include <cctype>
#include <cstdlib>
using namespace std;

class Menu {
public:
    int mno;
    char name[50], na[20];
    float price, dprice;
    char c[10], cho;

    void create_menu() {
        cout << "\n The item No. for The menu \n";
        mno = get_next_id();
        cout << "Generated Item Number: " << mno;
        cout << "\nItem is food or drink (f for food & d for drinks):" << endl;
        cin >> cho;

        if (cho == 'f' || cho == 'F') {
            cout << "\n\nPlease Enter The Name of The item ";
            cin.ignore();
            cin.getline(name, 50);

            cout << "\nPlease Enter The Price of The item ";
            cin >> price;
            while (price <= 0) {
                cout << "Price cannot be negative or zero. Please enter a valid price: ";
                cin >> price;
            }

            cout << "\nPlease Enter The Category of The item (veg/non-veg): ";
            cin >> c;

            while (strcmp(c, "veg") != 0 && strcmp(c, "non-veg") != 0) {
                cout << "Enter the right choice (veg/non-veg): ";
                cin >> c;
            }
            cout << "Item is created" << endl;
        }
        else if (cho == 'd' || cho == 'D') {
            cout << "\n\nPlease Enter The Name of The drink ";
            cin.ignore();
            cin.getline(na, 20);

            cout << "\nPlease Enter The Price of The drink ";
            cin >> dprice;
            while (dprice <= 0) {
                cout << "Price cannot be negative or zero. Please enter a valid price: ";
                cin >> dprice;
            }
            cout << "Item is created" << endl;
        }
        else {
            cout << "No item created" << endl;
            --mno;
        }
    }

    int get_next_id() {
        fstream file("Shop.dat", ios::in | ios::binary);

        if (!file) return 1;

        Menu temp;
        int max_id = 0;

        while (file.read((char*)&temp, sizeof(Menu))) {
            if (temp.retmno() > max_id) {
                max_id = temp.retmno();
            }
        }

        file.close();
        return max_id + 1;
    }

    void show_menu() {
        cout << "\nItem No.: " << mno;

        if (cho == 'f' || cho == 'F') {
            cout << "\nThe Name of The item: " << name;
            cout << "\nThe Price of The item: " << price;
            cout << "\nThe Category of The item: " << c;
        }
        else if (cho == 'd' || cho == 'D') {
            cout << "\nThe Name of The drink: " << na;
            cout << "\nThe Price of The drink: " << dprice;
        }
    }

    int retmno() {
        return mno;
    }

    float retprice() {
        if (cho == 'f' || cho == 'F') {
            return price;
        }
        else if (cho == 'd' || cho == 'D') {
            return dprice;
        }
        return 0;
    }

    char* retname() {
        if (cho == 'f' || cho == 'F') {
            return name;
        }
        else if (cho == 'd' || cho == 'D') {
            return na;
        }
        return 0;
    }

    const char* retc() const {
        if (cho == 'f' || cho == 'F') {
            return c;
        }
        return "Drink";
    }
};

fstream fp;
Menu mu;

bool containsOnlyAlphabet(const char* str) {
    for (int i = 0; i < strlen(str); ++i) {
        if (!isalpha(str[i]) && str[i] != ' ') {
            return false;
        }
    }
    return true;
}

class Consumer {
public:
    char name[20];
    long long mobile;
    int cust_id;
    static int next_id;

    Consumer() {
        cust_id = next_id++;
    }

    void getConsumerDetails() {
        cout << "\nEnter your name: ";
        cin.ignore();
        do {
            cin.getline(name, 20);

            if (containsOnlyAlphabet(name)) {
                break;
            }
            else {
                cout << "Invalid input. Name cannot contain numeric characters. Please enter a valid name: ";
            }
        } while (true);

        while (true) {
            cout << "Enter your mobile number (exactly 10 digits): ";
            cin >> mobile;

            if (cin.fail() || mobile < 1000000000 || mobile > 9999999999) {
                cin.clear();
                while (cin.get() != '\n');
                cout << "Invalid input. Please enter a 10-digit mobile number.\n";
            }
            else {
                break;
            }
        }
    }

    void displayConsumerDetails() {
        cout << "\n\n\t\t\t\t\t\t\t\t\t\t\t\t\t*INVOICE*\n";
        cout << "ID: " << cust_id << endl;
        cout << "Name: " << name << endl;
        cout << "Mobile number: " << mobile << endl;
    }
};

int Consumer::next_id = 1;

void writeConsumerDetails(Consumer consumer) {
    fstream consumerFile("Consumer.dat", ios::out | ios::app | ios::binary);
    consumerFile.write((char*)&consumer, sizeof(Consumer));
    consumerFile.close();
}

void displayConsumerDetails(int cust_id) {
    fstream consumerFile("Consumer.dat", ios::in | ios::binary);
    Consumer consumer;

    while (consumerFile.read((char*)&consumer, sizeof(Consumer))) {
        if (consumer.cust_id == cust_id) {
            consumer.displayConsumerDetails();
            break;
        }
    }
    consumerFile.close();
}

void write_menu() {
    fp.open("Shop.dat", ios::out | ios::app | ios::binary);
    mu.create_menu();
    fp.write((char*)&mu, sizeof(Menu));
    fp.close();
}

void display_all() {
    cout << "\n\n\n\t\tDISPLAY ALL RECORD !!!\n\n";
    fp.open("Shop.dat", ios::in | ios::binary);

    cout << "\n=========== FOOD ITEMS ===========\n";
    while (fp.read((char*)&mu, sizeof(Menu))) {
        if (mu.cho == 'f' || mu.cho == 'F') {
            mu.show_menu();
            cout << "\n====================================\n";
        }
    }

    fp.clear();
    fp.seekg(0, ios::beg);

    cout << "\n=========== DRINK ITEMS ===========\n";
    while (fp.read((char*)&mu, sizeof(Menu))) {
        if (mu.cho == 'd' || mu.cho == 'D') {
            mu.show_menu();
            cout << "\n====================================\n";
        }
    }
    fp.close();
}

void display_sp(int n) {
    int flag = 0;
    fp.open("Shop.dat", ios::in | ios::binary);
    while (fp.read((char*)&mu, sizeof(Menu))) {
        if (mu.retmno() == n) {
            mu.show_menu();
            flag = 1;
        }
    }
    fp.close();
    if (flag == 0)
        cout << "\n\nRecord does not exist";
}

void modify_menu() {
    int no, found = 0;
    char e;
    float price, dprice;
    char name[50], na[20];

    cout << "\n\n\tTo Modify ";
    cout << "\n\n\tPlease Enter The item you want to modify (d for drinks and f for food)" << endl;
    cin >> e;

    fp.open("Shop.dat", ios::in | ios::out | ios::binary);

    if (!fp) {
        cout << "File not found!" << endl;
        return;
    }

    cout << "Enter the item number" << endl;
    cin >> no;

    while (fp.read((char*)&mu, sizeof(Menu)) && found == 0) {
        if (mu.retmno() == no) {
            cout << "Please enter the new details of item " << no << endl;

            if (e == 'F' || e == 'f') {
                cout << "Enter the info you want to change:" << endl;
                cout << "1 for name\n2 for price\n3 for category\n4 for all\n" << endl;
                char h;
                cin >> h;

                if (h == '1') {
                    cout << "\n\nPlease Enter The New Name of The item ";
                    cin.ignore();
                    cin.getline(name, 50);
                    strcpy(mu.name, name);
                }
                else if (h == '2') {
                    cout << "\nPlease Enter The New Price of The item ";
                    cin >> price;
                    while (price <= 0) {
                        cout << "Price cannot be negative or zero. Please enter a valid price: ";
                        cin >> price;
                    }
                    mu.price = price;
                }
                else if (h == '3') {
                    cout << "\nPlease Enter The New Category of The item (veg/non-veg): ";
                    cin >> mu.c;
                }
                else if (h == '4') {
                    cout << "\n\nPlease Enter The New Name of The item ";
                    cin.ignore();
                    cin.getline(name, 50);
                    strcpy(mu.name, name);

                    cout << "\nPlease Enter The New Price of The item ";
                    cin >> price;
                    while (price <= 0) {
                        cout << "Price cannot be negative or zero. Please enter a valid price: ";
                        cin >> price;
                    }
                    mu.price = price;

                    cout << "\nPlease Enter The New Category of The item (veg/non-veg): ";
                    cin >> mu.c;
                }
                else {
                    cout << "Invalid choice";
                }
            }
            else if (e == 'D' || e == 'd') {
                cout << "Enter the info you want to change:" << endl;
                cout << "1 for name\n2 for price\n3 for all\n" << endl;
                char h;
                cin >> h;

                if (h == '1') {
                    cout << "\n\nPlease Enter The New Name of The drink ";
                    cin.ignore();
                    cin.getline(na, 20);
                    strcpy(mu.na, na);
                }
                else if (h == '2') {
                    cout << "\nPlease Enter The New Price of The drink ";
                    cin >> dprice;
                    while (dprice <= 0) {
                        cout << "Price cannot be negative or zero. Please enter a valid price: ";
                        cin >> dprice;
                    }
                    mu.dprice = dprice;
                }
                else if (h == '3') {
                    cout << "\nPlease Enter The New Name of The drink ";
                    cin.ignore();
                    cin.getline(na, 20);
                    strcpy(mu.na, na);

                    cout << "\nPlease Enter The New Price of The drink ";
                    cin >> dprice;
                    while (dprice <= 0) {
                        cout << "Price cannot be negative or zero. Please enter a valid price: ";
                        cin >> dprice;
                    }
                    mu.dprice = dprice;
                }
                else {
                    cout << "Invalid choice";
                }
            }
            else {
                cout << "Invalid choice";
            }

            int pos = -1 * static_cast<int>(sizeof(mu));
            fp.seekp(pos, ios::cur);
            fp.write((char*)&mu, sizeof(mu));
            cout << "\n\nRecord Updated";
            found = 1;
        }
    }
    fp.close();

    if (found == 0) {
        cout << "\n\nRecord Not Found ";
    }
}

void delete_menu() {
    int no;
    cout << "\n\n\n\tDelete Record";
    cout << "\n\nPlease Enter the item no. from The menu, You Want To Delete: ";
    cin >> no;
    fp.open("Shop.dat", ios::in | ios::binary);
    fstream fp2;
    fp2.open("Temp.dat", ios::out | ios::binary);
    fp.seekg(0, ios::beg);
    while (fp.read((char*)&mu, sizeof(Menu))) {
        if (mu.retmno() != no) {
            fp2.write((char*)&mu, sizeof(Menu));
        }
    }
    fp2.close();
    fp.close();
    remove("Shop.dat");
    rename("Temp.dat", "Shop.dat");
    cout << "\n\n\tRecord Deleted ..";
}

void menu() {
    fp.open("Shop.dat", ios::in | ios::binary);

    if (!fp) {
        cout << "ERROR!!! FILE COULD NOT BE OPEN\n\n\n Go To Admin Menu to create File";
        cout << "\n\n\n Program is closing ....";
        exit(0);
    }

    char a;
    cout << "\nChoose a category to display:\n";
    cout << "1. All Items\n";
    cout << "2. Vegetarian (Veg) Items\n";
    cout << "3. Non-Vegetarian (Non-Veg) Items\n";
    cout << "4. Drinks\n";
    cout << "Enter your choice (1-4): ";
    cin >> a;

    bool found = false;

    switch (a) {
    case '1':
        cout << "\n\nDISPLAYING ALL ITEMS\n";
        break;
    case '2':
        cout << "\n\nDISPLAYING VEGETARIAN (VEG) ITEMS\n";
        break;
    case '3':
        cout << "\n\nDISPLAYING NON-VEGETARIAN (NON-VEG) ITEMS\n";
        break;
    case '4':
        cout << "\n\nDISPLAYING DRINK ITEMS\n";
        break;
    default:
        cout << "Invalid choice\n";
        return;
    }

    while (fp.read((char*)&mu, sizeof(Menu))) {
        if ((a == '1') ||
            (a == '2' && (mu.cho == 'f' || mu.cho == 'F') && strcmp(mu.c, "veg") == 0) ||
            (a == '3' && (mu.cho == 'f' || mu.cho == 'F') && strcmp(mu.c, "non-veg") == 0) ||
            (a == '4' && (mu.cho == 'd' || mu.cho == 'D'))) {

            mu.show_menu();
            cout << "\n====================================\n";
            found = true;
        }
    }

    if (!found)
        cout << "No items found for the selected category." << endl;

    fp.close();
}

void clear_items() {
    fstream originalFile("Shop.dat", ios::in | ios::binary);
    fstream tempFile("Temp.dat", ios::out | ios::binary);

    if (!originalFile || !tempFile) {
        cout << "Error opening files!" << endl;
        return;
    }

    int itemNoToRemove;
    cout << "\nEnter the item number to clear: ";
    cin >> itemNoToRemove;

    originalFile.seekg(0, ios::beg);

    while (originalFile.read((char*)&mu, sizeof(Menu))) {
        if (mu.retmno() != itemNoToRemove) {
            tempFile.write((char*)&mu, sizeof(Menu));
        }
    }

    originalFile.close();
    tempFile.close();

    remove("Shop.dat");
    rename("Temp.dat", "Shop.dat");

    cout << "Items with item number " << itemNoToRemove << " cleared from the file." << endl;
}

void place_order() {
    Consumer consumer;
    consumer.getConsumerDetails();
    writeConsumerDetails(consumer);

    int cust_id = consumer.cust_id;

    cout << "\nPress Enter to view the menu...";
    cin.get();
    menu();

    char ch = 'Y';
    int order_arr[50], quan[50], c = 0;
    float amt, GST, ser, total = 0;

    while (ch == 'y' || ch == 'Y') {
        cout << "\n\nEnter The item No. from The menu: " << endl;
        cin >> order_arr[c];

        do {
            cout << "\nQuantity in number: " << endl;
            if (!(cin >> quan[c])) {
                cout << "Invalid input. Please enter a valid quantity." << endl;
                cin.clear();
                cin.ignore();
            }
            else if (quan[c] <= 0) {
                cout << "Quantity cannot be zero or negative. Please enter a valid quantity." << endl;
            }
            else {
                break;
            }
        } while (true);

        cout << "\nDo You Want To Order Another Product? (y/n)" << endl;
        cin >> ch;
        c++;
    }

    cout << "\n\nThank You For Placing The Order";
    displayConsumerDetails(cust_id);
    cout << "\nITEM No.\tITEM Name\t\tQuantity\tPrice\tAmount\tGST+Service Tax\n";
    cout << "--------------------------------------------------------------------------------\n";

    for (int x = 0; x < c; x++) {
        fp.open("Shop.dat", ios::in | ios::binary);
        bool found = false;
        
        while (fp.read((char*)&mu, sizeof(Menu))) {
            if (mu.retmno() == order_arr[x]) {
                amt = mu.retprice() * quan[x];
                GST = amt * 12 / 100;
                ser = amt * 6 / 100;
                cout << order_arr[x] << "\t" << mu.retname() << "\t\t" << quan[x] << "\t\t" << mu.retprice() << "\t" << amt << "\t" << GST + ser << endl;
                total += amt + GST + ser;
                found = true;
                break;
            }
        }
        fp.close();
        
        if (!found) {
            cout << "Item " << order_arr[x] << " not found in menu!" << endl;
        }
    }

    cout << "--------------------------------------------------------------------------------\n";
    cout << "\n\t\t\t\t\t\tTOTAL AMOUNT = " << total << endl;
    cout << "\n\t\t\t\t\t  (Including 12% GST and 6% Service Tax)\n";
}

void intro() {
    system("cls");
    cout << "\n============================================";
    cout << "\n\tRESTAURANT BILLING SYSTEM";
    cout << "\n============================================";
    cout << "\n\nMADE BY : 'AHANA GUPTA'";
    cout << "\n\nCOURSE : BCA-A";
    cout << "\n\nCOLLEGE : JCC";
    cout << "\n\nPress any key to continue...";
    getch();
}

void admin_menu() {
    char ch2;
    do {
        system("cls");
        cout << "\n\n\n\tADMIN MENU";
        cout << "\n\n\t1. CREATE MENU";
        cout << "\n\n\t2. DISPLAY ALL ITEMS";
        cout << "\n\n\t3. QUERY (Search Item)";
        cout << "\n\n\t4. MODIFY MENU";
        cout << "\n\n\t5. DELETE MENU";
        cout << "\n\n\t6. VIEW MENU (Customer View)";
        cout << "\n\n\t7. BACK TO MAIN MENU";
        cout << "\n\n\t8. CLEAR FILE (Delete Item)";
        cout << "\n\n\tPlease Enter Your Choice (1-8): ";
        cin >> ch2;
        
        switch (ch2) {
        case '1':
            write_menu();
            cout << "\n\nPress any key to continue...";
            getch();
            break;
        case '2':
            display_all();
            cout << "\n\nPress any key to continue...";
            getch();
            break;
        case '3': {
            display_all();
            int num;
            cout << "\n\n\tPlease Enter The item No.: ";
            cin >> num;
            display_sp(num);
            cout << "\n\nPress any key to continue...";
            getch();
            break;
        }
        case '4':
            display_all();
            modify_menu();
            cout << "\n\nPress any key to continue...";
            getch();
            break;
        case '5':
            display_all();
            delete_menu();
            cout << "\n\nPress any key to continue...";
            getch();
            break;
        case '6':
            menu();
            cout << "\n\nPress any key to continue...";
            getch();
            break;
        case '7':
            return;
        case '8':
            display_all();
            clear_items();
            cout << "\n\nPress any key to continue...";
            getch();
            break;
        default:
            cout << "\nInvalid choice! Press any key to continue...";
            getch();
        }
    } while (ch2 != '7');
}

void create_default_menu() {
    fstream file("Shop.dat", ios::in | ios::binary);

    if (file) {
        file.close();
        return;
    }

    file.close();
    fstream out("Shop.dat", ios::out | ios::binary);

    Menu m;

    // Food items - Veg
    m.mno = 1;
    strcpy(m.name, "Veg Burger");
    m.price = 100;
    m.cho = 'f';
    strcpy(m.c, "veg");
    out.write((char*)&m, sizeof(Menu));

    m.mno = 2;
    strcpy(m.name, "Margherita Pizza");
    m.price = 250;
    m.cho = 'f';
    strcpy(m.c, "veg");
    out.write((char*)&m, sizeof(Menu));

    m.mno = 3;
    strcpy(m.name, "Veg Noodles");
    m.price = 120;
    m.cho = 'f';
    strcpy(m.c, "veg");
    out.write((char*)&m, sizeof(Menu));

    // Food items - Non-Veg
    m.mno = 4;
    strcpy(m.name, "Chicken Burger");
    m.price = 150;
    m.cho = 'f';
    strcpy(m.c, "non-veg");
    out.write((char*)&m, sizeof(Menu));

    m.mno = 5;
    strcpy(m.name, "Chicken Pizza");
    m.price = 350;
    m.cho = 'f';
    strcpy(m.c, "non-veg");
    out.write((char*)&m, sizeof(Menu));

    m.mno = 6;
    strcpy(m.name, "Chicken Roll");
    m.price = 180;
    m.cho = 'f';
    strcpy(m.c, "non-veg");
    out.write((char*)&m, sizeof(Menu));

    // Drinks
    m.mno = 7;
    strcpy(m.na, "Coca Cola");
    m.dprice = 50;
    m.cho = 'd';
    out.write((char*)&m, sizeof(Menu));

    m.mno = 8;
    strcpy(m.na, "Pepsi");
    m.dprice = 50;
    m.cho = 'd';
    out.write((char*)&m, sizeof(Menu));

    m.mno = 9;
    strcpy(m.na, "Coffee");
    m.dprice = 80;
    m.cho = 'd';
    out.write((char*)&m, sizeof(Menu));

    m.mno = 10;
    strcpy(m.na, "Fresh Juice");
    m.dprice = 100;
    m.cho = 'd';
    out.write((char*)&m, sizeof(Menu));

    out.close();
    cout << "\nDefault menu created successfully!\n";
}

int main() {
    create_default_menu();

    char ch;
    intro();

    do {
        system("cls");
        cout << "\n\n\n\tMAIN MENU";
        cout << "\n\n\t01. CUSTOMER";
        cout << "\n\n\t02. ADMINISTRATOR";
        cout << "\n\n\t03. EXIT";
        cout << "\n\n\tPlease Select Your Option (1-3): ";
        cin >> ch;

        switch (ch) {
        case '1':
            place_order();
            cout << "\n\nPress any key to continue...";
            getch();
            break;

        case '2': {
            system("cls");
            int pass = 0;
            string enteredPassword = "";
            cout << "Enter admin password: ";

            while (true) {
                char input = _getch();

                if (input == '\r') {
                    break;
                }
                else if (input == '\b') {
                    if (pass > 0) {
                        cout << "\b \b";
                        pass--;
                        enteredPassword = enteredPassword.substr(0, enteredPassword.size() - 1);
                    }
                }
                else {
                    cout << '*';
                    pass++;
                    enteredPassword += input;
                }
            }

            string correctPassword = "2004";

            if (enteredPassword == correctPassword) {
                cout << "\nAccess Granted!" << endl;
                cout << "\nPress any key to continue...";
                getch();
                admin_menu();
            }
            else {
                cout << "\nAccess denied! Wrong password." << endl;
                cout << "\nPress any key to continue...";
                getch();
            }
            break;
        }

        case '3':
            cout << "\n\nThank you for using Restaurant Billing System!" << endl;
            exit(0);
            break;

        default:
            cout << "\nInvalid choice! Press any key to continue...";
            getch();
        }
    } while (ch != '3');

    return 0;
}
