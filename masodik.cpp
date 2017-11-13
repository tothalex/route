#include <iostream>
#include <string>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/algorithm.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include <vector>
#include <stdlib.h>

using namespace boost::filesystem;

const path p ("./City/Debrecen/");

struct data {
        int number;
        std::string file;
        std::string folder;
        std::vector<std::string> list;
};

int sum = 0;

void search( const path &search_here,std::vector<data>& v);
void print_v(std::vector<data>& v);
void build_v(boost::numeric::ublas::matrix<double>& m,std::vector<data>& v);
void print_matrix(boost::numeric::ublas::matrix<double>& m);
void build_vector(boost::numeric::ublas::vector<double>& vek,std::vector<data>& v);

int main(int argc, char const *argv[]) {

        std::vector<data> v;
        search(p,v);

        boost::numeric::ublas::matrix<double> m(v.size(),v.size());
        for (double i = 0; i < m.size1(); i++) {
                for (double j = 0; j < m.size2(); j++) {
                        m(i,j) = 0;
                }
        }
        build_v(m,v);
        //print_v(v);
        //print_matrix(m);

        boost::numeric::ublas::vector<double> vek(v.size());
        build_vector(vek,v);

        boost::numeric::ublas::vector<double> result(v.size());
        boost::numeric::ublas::vector<double> result2(v.size());

        bool init = true;
        axpy_prod(m,vek,result,init);
        for (size_t i = 0; i < 100; i++) {
                std::cout << result << '\n';
                axpy_prod(m,result,result2,init);
                result = result2;
        }

        return 0;
}

void search( const path &search_here,std::vector<data>& v)
{
        using namespace boost::filesystem;
        recursive_directory_iterator dir( search_here), end;
        while (dir != end)
        {
        if (is_regular_file(dir->path())) {
                std::string t = dir->path().string();
                size_t second_to_last = t.find_last_of("/", t.find_last_of("/")-1);
                std::string s = t.substr(second_to_last+1);
                std::string folder = s.substr(0,s.find("/"));
                data new_element;
                std::ifstream f(t);
                int n;
                std::string line;
                f>>n;
                sum += n;
                new_element.number = n;
                new_element.file = dir->path().filename().string();
                new_element.folder = folder;
                while (f>>line) {
                        size_t last = line.find_last_of("/");
                        size_t length = line.size() - last;
                        new_element.list.push_back(line.substr(last+1,length));
                }
                v.push_back(new_element);
        }
        ++dir;
        }
}

void  print_v(std::vector<data>& v) {

        using std::cout;
        for (size_t i = 0; i < v.size(); i++) {
                cout<<"Foglalkozas: "<<v[i].file<< '\n';
                cout<<"Foglalkozas szama: "<<v[i].number<< '\n';
                cout<<"Osztaly neve: "<<v[i].folder<<'\n';

                if (v[i].list.size() == 0) {
                        cout<<"---------------------------" <<'\n';
                }

                for (size_t j = 0; j < v[i].list.size(); j++) {
                        cout << "Tobbi foglalkozas: " << v[i].list[j] <<" "<< '\n';
                }
                cout << "---------------------------" << '\n';
        }

}

void build_v(boost::numeric::ublas::matrix<double>& m,std::vector<data>& v){

        for (double i = 0; i < v.size(); i++) {
                for (double j = 0; j < v[i].list.size(); j++) {
                        for (double k = 0; k < v.size(); k++) {
                                if (v[k].file == v[i].list[j]) {
                                        m(k,i) ++;
                                }
                        }
                }
        }

        double db;
        double insert;
        for (double i = 0; i < m.size1(); i++) {

                db = 0;
                for (double j = 0; j < m.size2(); j++) {
                        if (m(i,j) == 1) {
                                db++;
                        }
                }

                for (double j = 0; j < m.size2(); j++) {
                        if (m(i,j) == 1) {
                                insert = 1 / db;
                                m(i,j) = insert;
                        }
                }

        }

}

void print_matrix(boost::numeric::ublas::matrix<double>& m) {
        for (double i = 0; i < m.size1(); i++) {
                for (double j = 0; j < m.size2(); j++) {
                        std::cout << m(i,j) <<" ";
                }
                std::cout << '\n';
        }
}

void build_vector(boost::numeric::ublas::vector<double>& vek,std::vector<data>& v){

        for (double i = 0; i < v.size(); i++) {
                vek(i) = (double)v[i].number/sum;
        }

}
