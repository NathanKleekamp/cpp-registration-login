#include <iostream>
#include <iomanip>
#include <string>
#include <tuple>
#include <stdexcept>
#include <fstream>

using namespace std;

bool isInvalidAuthTokens(string username, string password) {
  bool isInvalid = username.find_first_of(" ") != string::npos || password.find_first_of(" ") != string::npos;

  if (isInvalid) {
    cout << "Usernames and passwords cannot contain spaces. Please try again.\n";
  }

  return isInvalid;
}

tuple<string, string> getUsernamePasswordText(int choice) {
  string usernameText { };
  string passwordText { };

  if (choice == 1) {
    usernameText = "Select a username: ";
    passwordText = "Select a password: ";
  } else if (choice == 2) {
    usernameText = "Username: ";
    passwordText = "Password: ";
  } else {
    throw invalid_argument("choice");
  }
  return make_tuple(usernameText, passwordText);
}

tuple<string, string> promptUser(int choice) {
  auto text = getUsernamePasswordText(choice);
  cout << get<0>(text);
  string username { };
  getline(cin >> ws, username);
  cout << get<1>(text);
  string password { };
  getline(cin >> ws, password);
  return make_tuple(username, password);
}

void writeFile(string data) {
  ofstream file;
  file.open("auth.txt");
  file << data;
  file.close();
}

tuple<string, string> readFile() {
  ifstream infile("auth.txt");
  string username { };
  string password { };
  infile >> username >> password;
  return make_tuple(username, password);
}

int compareTokens(string username, string password, string storedUsername, string storedPassword) {
  if (username == storedUsername && password == storedPassword) {
    cout << "You authenticated successfully.";
    return 0;
  } else {
    cout << "Incorrect username or password.";
    return 1;
  }
}

int main() {
  int choice { 0 };

  while(true) {
    if (choice == 0) {
      cout << "1. Register \n" << "2. Login\n" << "Your choice: ";
      cin >> choice;
    }

    if (choice == 1) {
      string username { };
      string password { };
      do {
        auto authTokens = promptUser(1);
        username = get<0>(authTokens);
        password = get<1>(authTokens);
      } while (isInvalidAuthTokens(username, password));
      writeFile(username + " " + password);
    } else if (choice == 2) {
      auto authTokens = promptUser(2);
      auto storedTokens = readFile();
      string username { get<0>(authTokens) };
      string password { get<1>(authTokens) };
      string storedUsername { get<0>(storedTokens) };
      string storedPassword { get<1>(storedTokens) };
      return compareTokens(username, password, storedUsername, storedPassword);
    } else if (cin.fail()) {
      cin.clear();
      cin.ignore();
      cout << "You must choose 1 or 2. \n";
    } else {
      cout << "You must choose 1 or 2. \n";
    }
    choice = 0;
    continue;
  }
  return 0;
}
