//10 variant
//Lab 6
//Не получается решить проблему segfault при открытии файла. Работает в отдельных файлах, не работает здесь

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

bool compare (struct student stud1, struct student stud2)
{
  int i = 0;
  for (i = 0; stud1.FCs[i] == stud2.FCs[i]; ++i);
   
  if (stud1.FCs[i] < stud2.FCs[i])
  return 1;
  else return 0;
}

float average (struct student stud, int num)
{
  float sum = 0, result = 0;
  for (int i = 0; i < num; ++i)
    sum += stud.subjects[i].grade;
  result = static_cast<float>(sum)/num;
  return result;
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
  out << '\0';
  return out;
}

std::string getl (std::string name) 
{
    std::fstream file(name);
    
    std::string str;
    std::getline(file, str, '\0');

    return str;
}

int main () 
{
  char answ = 0;
  std::string name_file;
  std::cout << "Do you want to read from a file or write information in it (r/w)? ";
  std::cin >> answ;
  std::cout << "Please, enter a name of file: ";
  std::cin >> name_file;
  std::fstream file;

  int num = 0, num2 = 0;
  std::vector<student> stud;
  std::vector<discipline> subj (num);

  if (answ == 'w')
  {
    file.open(name_file, std::ios::out);
    
    //Names of disciplines
    //int num = 0;
    std::cout << "Please, enter number of subjects: ";
    std::cin >> num;
    subj.resize(num);
    //std::vector<discipline> subj (num);
    for (int i = 0; i < num; ++i)
      std::cin >> subj[i].name;

    //Names, grades of students
    //int num2 = 0;
    std::cout << "Please, enter number of students: ";
    std::cin >> num2;
    stud.resize (num2);
    //std::vector<student> stud (num2);
    for (int i =0; i < num2; ++i) 
    {
      std::cin.ignore(256, '\n');
      std::cout << "Please, enter first and second name of a student: ";
      std::getline (std::cin, stud[i].FCs);
  
      stud[i].subjects.resize (num);
      for (int k = 0; k < num; ++k)
      {
        stud[i].subjects[k].name = subj[k].name; 
        std::cout << "Please, enter " << stud[i].FCs << "'s grade:\n";
        std::cout << stud[i].subjects[k].name << " : ";
        std::cin >> stud[i].subjects[k].grade;
      }
    }
  } else {

    //file.open(name_file, std::ios::in);
    std::string file_str;
    //std::getline(file, file_str,'\0');

    file_str = getl(name_file);

    //Analazing of the file
    //Student's name
    int pos = 0, last = 0, it = 0;
    while (pos != -1) 
    {
      pos = file_str.find(" = ");
      last = file_str.find('\n');
      if (last != -1) 
        stud[it].FCs = file_str.substr(last, pos - last);
      else stud[it].FCs = file_str.substr(0, pos);
      ++it;
    }
    it = num2; 
    last = 0;

    //Desciplines
    char k = 0;
    int i = 0, l = 0;
    for (k = file_str[i]; k != '\n'; ++i)
    {
      if (k == ':')
      {
        last = file_str.rfind(' ');
        subj[l].name = file_str.substr(last + 1, i - last - 1);
        ++l;
      }
    }
    l = num;
  }
      
  //Sorting
  std::sort(stud.begin(), stud.end(), compare); 

  //Average grade
  std::vector<float> aver(num2);
  int num_of_max = 0;
  for (int i = 0; i < num2; ++i)
  {
    aver[i] = average(stud[i], num); 
    std::cout << i+1 << ". " <<  stud[i].FCs << "\t Grade: " << aver[i] << '\n';
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

  //Writing to database
  file << stud;
  file.close();

  return 0;
}

