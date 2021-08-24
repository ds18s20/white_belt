//============================================================================
// Name        : white_belt.cpp
// Author      : Dima
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <sstream> // для stringstream
#include <exception>
#include <iomanip>
#include <algorithm>
using namespace std;

vector<string> log_out;

class Date
{
public:
    Date()
    {
        year = 0;
        month = 1;
        day = 1;
    }
    Date(int y,int m, int d)
    {
        year = y;
        month = m;
        day = d;
    }
    int GetYear() const
    {
        return year;
    }
    int GetMonth() const
    {
        return month;
    }
    int GetDay() const
    {
        return day;
    }
    friend bool operator<(const Date& lhs, const Date& rhs);
    friend bool operator>(const Date& lhs, const Date& rhs);
private:
    int year;
    int month;
    int day;
};

ostream& operator<<(ostream& stream,const Date& date)
{
    stream  << setfill('0');
    stream  <<setw(4)<<date.GetYear()<<'-'
            <<setw(2)<<date.GetMonth()<<'-'
            <<setw(2)<<date.GetDay();

    return stream;
}

int input_date( istringstream& in,string& str,char next)
{
    int y;
    string str_y, str_m, str_d;


    char c = in.peek();
    if (c=='-')
    {
        c = in.get();
        char e = in.peek();
        if (e=='-')
            throw std::runtime_error("Wrong date format: "+str);
        else if(e=='+')
        {
            e = in.get();
            try
            {
                std::getline(in,str_y,next);
                istringstream inp(str_y);
                inp>>y;
            }
            catch (std::invalid_argument const& ex)
            {
                throw std::runtime_error("Wrong date format: "+str);
            }
        }
        else
        {
            try
            {
                std::getline(in,str_y,next);
                istringstream inp(str_y);
                inp>>y;
                y=-y;
            }
            catch (std::invalid_argument const& ex)
            {
                throw std::runtime_error("Wrong date format: "+str);
            }
        }
    }
    else if (c=='+')
    {
        c = in.get();
        char e = in.peek();
        if (e=='-')
            e = in.get();
        try
        {
            std::getline(in,str_y,next);
            istringstream inp(str_y);
            inp>>y;
            y = abs(y);

        }
        catch (std::invalid_argument const& ex)
        {
            throw std::runtime_error("Wrong date format: "+str);
        }
    }
    else
    {
        try
        {
            std::getline(in,str_y,next);

            istringstream inp(str_y);
            inp>>y;
            y = abs(y);

        }
        catch (std::invalid_argument const& ex)
        {
            throw std::runtime_error("Wrong date format: "+str);
        }

    }
    return y;
}


istream& operator>>(istream& stream,Date& date)
{
    char c1,c2;
    int y, m,d;
    string str;
    string str_y, str_m, str_d;

    stream >>str;
    istringstream in(str);
    if (in)
    {
        y=input_date(in,str,'-');
        m=input_date(in,str,'-');
        d=input_date(in,str,in.eof());
     //   cout<<"INPUT: "<<y<<" "<<m<<" "<<d<<endl;
        if (m>=1 && m<=12)
        {
            if (d>=1 && d<=31)
                date = Date(y, m,d);
            else
                throw std::runtime_error("Day value is invalid: "+std::to_string(d));
        }
        else
            throw std::runtime_error("Month value is invalid: "+std::to_string(m));
    }
    else
        throw std::runtime_error("Wrong date format: "+str);

    return stream;
}

bool operator>(const Date& lhs, const Date& rhs)
{
    if (lhs.year-rhs.year>0)
        return true;
    else if (lhs.year-rhs.year<0)
        return false;
    else if (lhs.year-rhs.year==0)
    {
        if (lhs.month - rhs.month>0)
            return true;
        else if (lhs.month - rhs.month<0)
            return false;
        else if (lhs.month - rhs.month == 0)
        {
            if (lhs.day - rhs.day>0)
                return true;
            else
                return false;
        }
    }
    return false;
}



bool operator<(const Date& lhs, const Date& rhs)
{
    if (lhs.year-rhs.year<0)
        return true;
    else if (lhs.year-rhs.year>0)
        return false;
    else if (lhs.year-rhs.year==0)
    {
        if (lhs.month - rhs.month<0)
            return true;
        else if (lhs.month - rhs.month>0)
            return false;
        else if (lhs.month - rhs.month == 0)
        {
            if (lhs.day - rhs.day<0)
                return true;
            else
                return false;
        }
    }
    return false;
}
bool operator==(const Date& lhs, const Date& rhs)
{
    return (lhs.GetYear() == rhs.GetYear() && lhs.GetMonth() == rhs.GetMonth() && lhs.GetDay() == rhs.GetDay());
}

class Database
{
public:
    Database()
    {
    }
    void AddEvent(const Date& date, const string& event)
    {
        if (dateEvent.count(date)>0)
        {
            set<string> list = dateEvent.at(date);
            list.insert(event);
            dateEvent[date] = list;
        }
        else
        {
            set<string> list;
            list.insert(event);
            dateEvent[date] = list;
        }
    }
    bool DeleteEvent(const Date& date, const string& event)
    {
        if (dateEvent.count(date)>0)
        {
            set<string>& list = dateEvent.at(date);
            if (list.count(event))
            {
                auto i = std::find(list.begin(), list.end(), event);
                list.erase(i);
                cout<<"Deleted successfully"<<endl;
                return true;
            }
            else
            {
                cout<<"Event not found"<<endl;
                return false;
            }
        }
        else
        {
            cout<<"Event not found"<<endl;
            return false;
        }
    }
    int  DeleteDate(const Date& date)
    {
        int N =0;
        if (dateEvent.count(date)>0)
        {
            N = dateEvent.at(date).size();
            dateEvent.erase(date);
        }
        cout<<"Deleted "<<N<<" events"<<endl;
        return N;
    }

    set<string>& Find(const Date& date) const
    {
        set<string> list;
        if (dateEvent.count(date)>0)
        {
            list = dateEvent.at(date);
            for (auto i: list)
                cout<<i<<endl;
        }
        return list;
    }

    void Print() const
    {
        for (auto key : dateEvent)
        {
            set<string> list = key.second;
            for (auto i : list)
            {
                cout<<key.first<<" "<<i<<endl;
            }
        }
    }
private:
    map <Date,set<string>> dateEvent;
    map <Date,set<string>> _dateEvent;
};

int main()
{
    Database db;

    string command;
    string cmd;

    while (getline(cin, command))
    {
        istringstream input(command);
        if (command!="")
        {
            input>>cmd;
            if (cmd == "Add")
            {
                string date, event;
                input>>date>>event;

                Date d;
                try
                {
                    istringstream in(date);
                    in>>d;
                    db.AddEvent(d,event);
                }
                catch (const runtime_error& ex)
                {
                    cout<<ex.what();
                    return 1;
                }

            }
            else if(cmd == "Del")
            {
                string date, event;
                input>>date>>event;
                Date d;
                try
                {
                    istringstream in(date);
                    in>>d;
                    if (!event.size())
                        db.DeleteDate(d);
                    else
                        db.DeleteEvent(d,event);
                }
                catch (const runtime_error& ex)
                {
                    cout<<ex.what();
                    return 1;
                }
            }
            else if (cmd == "Find")
            {
                string date;
                input>>date;
                Date d;
                try
                {
                    istringstream in(date);
                    in>>d;
                    db.Find(d);

                }
                catch (const runtime_error& ex)
                {
                    cout<<ex.what();
                    return 1;
                }
            }
            else if (cmd == "Print")
            {
                db.Print();
            }
            else if (cmd == "\n")
            {
            }
            else
            {
                cout<<"Unknown command: "<<cmd<<endl;
            }

        }
    }

    return 0;
}
