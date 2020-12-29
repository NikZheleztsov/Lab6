//10 variant
//Lab 6

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>

struct discipline
{
    std::string name;
    int grade = 0;
};


struct student
{
    std::string FCs;
    std::vector<discipline> subjects;
};

bool compare (student stud1, student stud2)
{
    int i = 0;
    for (i = 0; stud1.FCs[i] == stud2.FCs[i]; ++i);

    return (stud1.FCs[i] < stud2.FCs[i]);
}

float average (student stud, int num)
{
    float sum = 0, result = 0;
    for (int i = 0; i < num; ++i)
        sum += stud.subjects[i].grade;
    result = static_cast<float>(sum)/num;
    return result;
}

void str_parsing (std::vector<student>& stud, std::string& file_str)
{
    int num = 0, num2 = 0;
    std::vector<discipline> subj (num);
    //Analazing of the file
    //Student's name
    int pos = 0, last = 0, it = -1;
    while (pos != -1)
    {
        pos = file_str.find('\n', pos + 1);
        it++;
    }

    stud.resize(it);
    pos = 0, it = 0;

    while (pos != -1) 
    {
        pos = file_str.find(" = ", pos + 1);
        last = file_str.rfind('\n', pos);
        if (last != -1 && pos != -1 ) 
            stud[it].FCs = file_str.substr(last + 1, pos - last - 1);
        else if (pos != -1) stud[it].FCs = file_str.substr(0, pos);
        it++;
    }
    num2 = it - 1;  //number of students
    it = 0;

    //Desciplines
    char k = 0;
    int i = 0, l = 0;
    last = file_str.find('\n', 0);
    for (i = 0; i < last; i++)
    {
        k = file_str[i];
        if (k == ':')
        {
            it++;
        }
    }
    subj.resize(it);

    int grades[] = {'=', '1', '2', '3', '4', '5'};
    for (i = 0; i < last; i++)
    {
        k = file_str[i];
        if (k == ':')
        {
            int counter = 0;
            pos = file_str.rfind(' ', i);

            for (int j = 0; j < 6; j++)   // several words in the name of the subject
                if (file_str[pos - 1] == grades[j])
                    counter++;
            
            if (counter == 0)
                pos = file_str.rfind(' ', pos - 1);

            subj[l].name  = file_str.substr(pos + 1, i - pos - 1);
            l++;
        } 
    }

    int new_pos = 0,
        a = 0;

    for (int l = 0; l < num2; ++l) //students
    {
        stud[l].subjects.resize(it);
        for (int i = 0; i < it; i++) //subj
            stud[l].subjects[i].name = subj[i].name;

        for (int i = new_pos; i < last; i++)
        {
            k = file_str[i];
            if (k == ':')
            {
                pos = file_str.find(' ', i);
                stud[l].subjects[a].grade = std::stoi(file_str.substr(i+1, pos - i - 1));
                a++;
            }
            if (a == 3) break;
        }
        new_pos = last + 1;
        last = file_str.find('\n', last + 1);
        a = 0;
    }

}

std::ostream& operator << (std::ostream& out, std::vector<student>& stud)
{
    for (int i = 0; i < stud.size(); ++i)
    {
        out << stud[i].FCs << " = ";
        for (int k = 0; k < stud[i].subjects.size(); ++k)
            out << stud[i].subjects[k].name << ':' << stud[i].subjects[k].grade << ' ';
        out << '\n';
    }
    return out;
}

std::istream& operator >> (std::istream& file, std::vector<student>& stud)
{
    std::string file_str;
    std::getline(file, file_str,'\0');
    str_parsing(stud, file_str);

    return file;
}

void read (std::vector<student>& stud, std::string name_file, int num)
{
    std::ifstream file (name_file, std::ios::binary | std::ios::in | std::ios::ate);

    if(file.is_open()) {

        //int size = file.tellg();
        file.seekg(0, std::ios::beg);
        stud.resize(num);

        //char* memblock = new char [size];
        //std::string file_str;
        //std::string memblock (size, '\0');

        //short size;
        //file.read(reinterpret_cast<char*>(&size), sizeof(short));
        //file.read(reinterpret_cast<char*>(&stud[0].FCs), size);

        //char* memblock = new char [size];
        //student* arr = new student [num];
        
        file.read (reinterpret_cast<char*>(&stud[0]), num * sizeof(student)); //инвалидация указателя

        //file_str = static_cast<std::string>(memblock);
        //int str_size = static_cast<std::string>(memblock).size();
        //delete [] memblock;
        //for (int i = 0; i < num; i++)
        //    stud.push_back(arr[i]);

        //str_parsing(stud, memblock);

        file.close();

    } else std::cout << "Unable to open file\n";
}

void write (std::ostream& out, std::vector<student> stud) 
{
    //student* arr = new student [stud.size()];
    //std::copy(stud.begin(), stud.end(), arr);

    out.write(reinterpret_cast<const char*>(&stud[0]), stud.size() * sizeof(student));

    /*
    short size = sizeof(stud[0].FCs);
    out.write(reinterpret_cast<char*>(&size), sizeof(short));
    out.write(reinterpret_cast<char*>(&stud[0].FCs), size);
    */
}

int main () 
{
    char answ = 0;
    std::string name_file;
    std::cout << "Do you want to read from a file or write information in it?\nOr maybe you want to work with binary files? (r/w/b) ";
    std::cin >> answ;
    std::cout << "Please, enter a name of file: ";
    std::cin >> name_file;
    std::fstream file;

    int num = 0, num2 = 0;
    std::vector<student> stud;
    std::vector<discipline> subj (num);
    std::string file_str;

    if (answ == 'w')
    {
        file.open(name_file, std::ios::out);

        if (file.is_open()) {

            //Names of disciplines
            std::cout << "Please, enter number of subjects: ";
            std::cin >> num;
            subj.resize(num);
            std::cout << "Please, enter names of subjects:\n";
            std::cin.ignore(100, '\n');
            for (int i = 0; i < num; ++i)
                std::getline(std::cin, subj[i].name);

            //Names, grades of students
            std::cout << "Please, enter number of students: ";
            std::cin >> num2;
            stud.resize (num2);
            for (int i =0; i < num2; ++i) 
            {
                std::cin.ignore(256, '\n');
                std::cout << "Please, enter first and second name of a student: ";
                std::getline (std::cin, stud[i].FCs);

                stud[i].subjects.resize (num);
                for (int k = 0; k < num; ++k)
                {
                    stud[i].subjects[k].name = subj[k].name;  //copying of subj
                    std::cout << "Please, enter " << stud[i].FCs << "'s grade:\n";
                    std::cout << stud[i].subjects[k].name << " : ";
                    std::cin >> stud[i].subjects[k].grade;
                }
            }

            file << stud;

        } else std::cout << "Unable to open file\n";

    } else if (answ == 'r')
    {
        file.open(name_file, std::ios::in);

        if(file.is_open()) {

            file >> stud;

        } else std::cout << "Unable to open file\n";

    } else if (answ == 'b')
    {
        std::cout << "Please, enter number of students: ";
        int num;
        std::cin >> num;
        read(stud, name_file, num);

    } else 
        std::cout << "Unknown command\n";

    if (file.is_open()) {

        std::cout << stud;
        num2 = stud.size();
        num = stud[0].subjects.size();

        //Sorting
        std::sort(stud.begin(), stud.end(), compare); 

        //Average grade
        std::vector<float> aver(num2);
        int num_of_max = 0;
        for (int i = 0; i < num2; ++i)
        {
            aver[i] = average(stud[i], num); 
            std::cout << i+1 << ". " <<  stud[i].FCs <<" | " <<  "Grade: " << aver[i] << std::endl;
        }

        //The best one
        std::cout << "The best student(s) in the group: "; 
        for (int k = 0; k < num2 - 1; ++k)
        {
            if (average(stud[k], num) < average(stud[k+1], num)) 
                num_of_max = k+1;
        }

        //Anyone else with the same grade
        std::cout << stud[num_of_max].FCs; 
        for (int i = 0; i < num2; i++)
            if (i != num_of_max && average(stud[i],num) == average(stud[num_of_max],num))
                std::cout << ", " << stud[i].FCs;

        std::cout << std::endl;

        file.close();

        //Writing to binary
        char new_answ = 0;
        std::string new_name_file;
        std::cout << "Do you want to write your database to a binary file? (y/n) ";
        std::cin >> new_answ;
        if (new_answ == 'y' )
        {
            std::cout << "Please, enter a name of a file: ";
            std::cin >> new_name_file;
            std::ofstream out (new_name_file, std::ios::binary);

            //file.open(name_file, std::ios::in);
            //std::getline(file, file_str,'\0');

            write(out, stud);

            out.close(); 

            return 0;

        } 
    }

    return 0;
}

