# 🏦 Bank System Extension (Project 2)
Overview
This is an extended version of the Bank Management System in C++. In addition to basic client operations, this version introduces bank transactions including deposits, withdrawals, and total balances.

All client data is stored in a text file (Clients.txt) to ensure persistence.
___
 ⚡ Features 
 
 📋 List All Clients: Display all clients with full details.
 
 ➕ Add New Client: Add clients with unique account numbers.
 
 ❌ Delete Client: Remove clients from the system.
 
 ✏️ Update Client: Update client information.
 
 🔍 Find Client: Search for a client by account number.
 
 💵 Transactions: Deposit, withdraw, and view total balances.
 
 🚪 Exit: Exit the program safely.
 
 💾 Data Storage

Client records are stored in Clients.txt.

Each record uses the delimiter: #//#.

Deleted clients are permanently removed from the file.

# 🛠️ Code Structure
Structs: 1 (stClient)

Enums: 2 (enMainMenueOptions, enTranactionsOptions)

Functions: 38
The program maintains client data in a vector and ensures all changes are reflected in the file after each operation.

# 🏁 How It Works
The program displays the Main Menu with seven options.

Users select operations (list, add, delete, update, find, transactions, exit).

The Transactions menu allows deposits, withdrawals, and checking total balances.

All actions are validated to avoid invalid operations or duplicate account numbers.


# 🌟 Future Improvements
Adding transaction history logs for each client.

Implementing staff authentication and access control.

Transitioning to a fully OOP design for better maintainability.

Developing a GUI version for a more user-friendly interface.
