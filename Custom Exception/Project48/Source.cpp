#include<iostream>
#include<ctime>
#include<windows.h>
#include<string>
using namespace std;


class Exception{
protected:
	string text;
	int line;
	string source;
	string created;
	Exception(const string& text, const int line
	 ,const string& source,const string & created)
	{
		SetText(text);
		SetLine(line);
		SetSource(source);
		SetTime(created);
	}
public:
	void SetText(const string text) {
		this->text = text;
	}
	void SetLine(const int line) {
		this->line = line;
	}
	void SetSource(const string source) {
		this->source = source;
	}
	void SetTime(const string created) {
		time_t currently = time(0);
		char dt[30]{};
		ctime_s(dt, sizeof dt, &currently);
		string time = dt;

		this->created = dt;
	}
	void PrintMsg()const {
		cout << "------ Exception Info ------" << endl;
		cout << "Text:" << text << endl;
		cout << "Line:" << line << endl;
		cout << "Source:" << source << endl;
		cout << "Time:" << created << endl;
	}
}; 
class DatabaseException :public Exception {
public:
	DatabaseException(const string& text, const int line
	 ,const string& source,const string& created)
	:Exception(text,line,source,created)
	{

	}
};
class InvalidArgumentException :public Exception {
public:
	InvalidArgumentException(const string& text, const int line
	 ,const string& source, const string& created)
	 :Exception(text, line, source,created)
	{

	}
};

class User {
	string username;
	string password;
	string name;
	string surname;
	int id;
public:
	User() :id(),username(), password(), name(), surname() {};
	User(const int id,const string& username,const string& password,const string&name,const string&surname)
		:User()
	{
		SetId(id);
		SetUsername(username);
		SetPassword(password);
		SetName(name);
		SetSurname(surname);
	}
	int GetId()const {
		return id;
	}
	string GetUsername()const{
		return username;
	}
	string GetPassword()const{
		return password;
	}
	string GetName()const {
		return name;
	}
	string GetSurname()const {
		return surname;
	}

	void SetId(const int id) {
		if (id < 0) {
			throw InvalidArgumentException("User Id smaller than zero", __LINE__, "Source.cpp", __TIME__);
		}
		this->id = id;
	}
	void SetUsername(const string&username) {
		if (username == "") {
			throw InvalidArgumentException("Username is NULL",__LINE__, "Source.cpp",__TIME__);
		}
		if (username.length() < 6) {
			throw InvalidArgumentException("Username length smaller than 6", __LINE__, "Source.cpp", __TIME__);
		}
		if (username.at(0) > 96 && username.at(0) < 123) {
			throw InvalidArgumentException("Username first symbol is is a lowercase letter", __LINE__, "Source.cpp", __TIME__);
		}
		this->username = username;
	}
	void SetPassword(const string& password) {
		if (password == "") {
			throw InvalidArgumentException("Password is NULL", __LINE__, "Source.cpp", __TIME__);
		}
		if (password.length() < 6) {
			throw InvalidArgumentException("Password length smaller than 6", __LINE__, "Source.cpp", __TIME__);
		}
		this->password = password;
	}
	void SetName(const string& name) {
		if (name == "") {
			throw InvalidArgumentException("Name is NULL", __LINE__, "Source.cpp", __TIME__);
		}
		if (name.length() < 4) {
			throw  InvalidArgumentException("Name length smaller than 4", __LINE__, "Source.cpp", __TIME__);
		}
		this->name = name;
	}
	void SetSurname(const string& surname) {
		if (surname == "") {
			throw InvalidArgumentException("Surname is NULL", __LINE__, "Source.cpp", __TIME__);
		}
		if (surname.length() < 4) {
			throw  InvalidArgumentException("Surname length smaller than 4", __LINE__, "Source.cpp", __TIME__);
		}
		this->surname = surname;
	}
	
	
	void show()const {
		cout << "Id->" << id << endl;
		cout << "Username->" << username << endl;
		cout << "Password->" << password << endl;
		cout << "Name->" << name << endl;
		cout << "Surname->" << surname << endl;
	}
		
};

class Database {
	User** users = NULL;
	int user_count=0;
public:
	Database() = default;
	Database(User **users, const int user_count)
	{
		setUserCount(user_count);
		setUsers(users);
	}

	int getUserCount()const {
		return user_count;
	}
	User** getUsers()const {
		return users;
	}

	void setUserCount(const int user_count) {
		if (user_count < 0) throw InvalidArgumentException("User count can't be negative",__LINE__, "Source.cpp",__TIME__);
		this->user_count = user_count;
	}
	void setUsers(User * *users) {
		if (users == NULL) throw InvalidArgumentException("Users NULL",__LINE__, "Source.cpp",__TIME__);
		this->users = users;
	}

	void addUser( User& user) {
		for (int i = 0; i < getUserCount(); i++)
		{
			if (users[i]->GetUsername() == user.GetUsername()) {
				throw InvalidArgumentException("This username is already have", __LINE__, "Source.cpp", __TIME__);
			}
			
		}
		User** newusers = new User * [user_count + 1];
		for (int i = 0; i < user_count; i++)
		{
			newusers[i] = users[i];
		}
		newusers[user_count] = &user;

		delete[] users;
		users = newusers;
		newusers = nullptr;
		user_count++;
		cout << "User adding Database..." << endl;
		Sleep(1000);
	}
	User& getUserByUsername(string username) {
		for (size_t i = 0; i < user_count; i++)
		{
			if ( users[i]->GetUsername()==username) {
				return*(users[i]);
			}
		}
		throw InvalidArgumentException("This user not found", __LINE__, "Source.cpp", __TIME__);
	}
	void updateUser(User& olduser, const User& newuser) {
		for (int i = 0; i < user_count; i++)
		{
			if (users[i] == &olduser) {
				olduser.SetName(newuser.GetName());
				olduser.SetSurname(newuser.GetSurname());
				olduser.SetPassword(newuser.GetPassword());
				olduser.SetUsername(newuser.GetUsername());

				cout << "User is Update" << endl;
				return;
			}
		}
		throw InvalidArgumentException("This user is not found in Database", __LINE__, "Source.cpp", __TIME__);
		
	}
	void deleteUserById(const int& id) {
		for (int i = 0; i < user_count; i++)
		{
			if (users[i]->GetId() == id) {
				auto newuser = new User * [user_count - 1];
				for (int k = 0,j=0; j <user_count ; j++)
				{
					if (j != i)
						newuser[k++] = users[j];
				}
				delete[] users;
				users = newuser;
				newuser = nullptr;
				user_count--;
				return;
			}
		}
		throw InvalidArgumentException("This user is not found", __LINE__ ,"Source.cpp", __TIME__);
	}
	size_t hashcode(const string& data) {
		size_t h(0);
		for (int i = 0; i < data.length(); i++)
			h = (h << 6) ^ (h >> 26) ^ data[i];
		return h;
	}
	void hashUserDataById(const int& id) {
		for (int i = 0; i < user_count; i++)
		{
			if (users[i]->GetId() == id) {

				size_t hashname = hashcode(users[i]->GetName());
				users[i]->SetName(to_string(hashname));
				size_t hashpassword = hashcode(users[i]->GetPassword());
				users[i]->SetPassword(to_string(hashpassword));
				size_t hashsurname = hashcode(users[i]->GetSurname());
				users[i]->SetSurname(to_string(hashsurname));
				size_t hashusername = hashcode(users[i]->GetUsername());
				users[i]->SetUsername(to_string(hashusername));
				return;
			}
		}
		throw InvalidArgumentException("This user is not found", __LINE__, "Source.cpp", __TIME__);
	}

	void showUsers() {
		cout << "----------- USERS INFO ---------" << endl;
		for (int i = 0; i < user_count; i++)
		{
			cout << "Name->" << users[i]->GetName() << endl;
			cout << "Surname->" << users[i]->GetSurname() << endl;
			cout << "Username->" << users[i]->GetUsername() << endl;
			cout << "Password->" << users[i]->GetPassword() << endl << endl;
			
		}
	}
};

class Registration {
	Database _database;
public:
	Registration() :_database() {};
	Registration(const Database& database) {
		_database = database;
	}
	Database& GetDatabase() {
		return _database;
	}

	void signIn(string username, string password) {
		for (int i = 0; i < _database.getUserCount(); i++)
		{
		  if (_database.getUsers()[i]->GetUsername() == username && _database.getUsers()[i]->GetPassword() == password) {
				cout << "User Sign In" << endl;
				return;
		  }
		  else if (_database.getUsers()[i]->GetUsername() == username && _database.getUsers()[i]->GetPassword() != password) {
			  throw DatabaseException("Sign in is not possible:Password is wrong", __LINE__, "Source.cpp", __TIME__);
		  }
		}
		throw DatabaseException("This user is not available", __LINE__, "Source.cpp", __TIME__);
	}


	//Eger istifadechi varsa hemen usernamede throw DatabaseException
	
	void signUp(int id,string username, string password, string name, string surname) {
		for (int i = 0; i < _database.getUserCount(); i++)
		{
			if (_database.getUsers()[i]->GetUsername() == username) {
				throw DatabaseException("Sign up is not possible:This username is already have", __LINE__, "Source.cpp", __TIME__);
			}
		}
		User* newUser = new User[1];
		newUser->SetId(id);
		newUser->SetUsername(username);
		newUser->SetPassword(password);
		newUser->SetName(name);
		newUser->SetSurname(surname);

		GetDatabase().addUser(*newUser);
	}
};

class System {
public:
	static void Control() {
		
		Database db;
		Registration twitter(db);
		try
		{

         User user1(1,"SumqayitNebi", "Nebi123", "Nebiii", "Nebili");
		 User user2(2,"SumqayitEmiraslan", "6565685", "Emiraslan", "Aliyev");
		 twitter.GetDatabase().addUser(user1);
		 twitter.GetDatabase().addUser(user2);
		 twitter.GetDatabase().showUsers();

		 ////twitter.GetDatabase().deleteUserById(2); Silib yoxlama ede bilersiz silinmis data update olunmur

		 //cout << "------- After Update "<<user2.GetUsername()<<" USER INFO --------" << endl;
		 //User user3(3,"XirdalanKenan", "Kenan6543", "Kenann", "Idayetov");
		 //twitter.GetDatabase().updateUser(user2, user3);
		 //twitter.GetDatabase().showUsers();

		 //twitter.signIn("SumqayitNebi", "Nebi123");

		 /*cout << "------- After Sign Up User  --------" << endl;
		 twitter.signUp(3,"XirdalanKenan", "Kenan6543", "Kenann", "Idayetov");
		 twitter.GetDatabase().showUsers();*/

		 /*cout << "------- After Delete User by Id --------" << endl;
		 twitter.GetDatabase().deleteUserById(1);
		 twitter.GetDatabase().showUsers();*/ 

		 /*cout << "------- After Hash User By Id --------" << endl;
		 twitter.GetDatabase().hashUserDataById(2);
		 twitter.GetDatabase().hashUserDataById(1);
		 twitter.GetDatabase().showUsers();*/
		}
		catch (InvalidArgumentException ex)
		{
			 ex.PrintMsg();
		}
		catch (DatabaseException ex) {
			ex.PrintMsg();
		}
	}

};

void main() {
	System::Control();
}



