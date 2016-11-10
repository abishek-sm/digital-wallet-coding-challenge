#include "Snap.h"
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include<stdio.h>

#define EPSILON 0.0001
#define DIRNAME "bfsdfs"
#define NNodes 90000

using namespace TSnap;

template <typename T> struct is_signed_int { enum { val = false }; };
template <> struct is_signed_int<short> { enum { val = true}; };
template <> struct is_signed_int<int> { enum { val = true}; };
template <> struct is_signed_int<long> { enum { val = true}; };
template <> struct is_signed_int<long long> { enum { val = true}; };

template <typename T> struct is_unsigned_int { enum { val = false }; };
template <> struct is_unsigned_int<unsigned short> { enum { val = true}; };
template <> struct is_unsigned_int<unsigned int> { enum { val = true}; };
template <> struct is_unsigned_int<unsigned long> { enum { val = true}; };
template <> struct is_unsigned_int<unsigned long long> { enum { val = true}; };

template <typename T> struct is_int {
    enum { val = (is_signed_int<T>::val || is_unsigned_int<T>::val) };
};

struct csv_istream {
    std::istream &is_;
    csv_istream (std::istream &is) : is_(is) {}
    void scan_ws () const {
        while (is_.good()) {
            int c = is_.peek();
            if (c != ' ' && c != '\t') break;
            is_.get();
        }
    }
    void scan (std::string *s = 0) const {
        std::string ws;
        int c = is_.get();
        if (is_.good()) {
            do {
                if (c == ',' || c == '\n') break;
                if (s) {
                    ws += c;
                    if (c != ' ' && c != '\t') {
                        *s += ws;
                        ws.clear();
                    }
                }
                c = is_.get();
            } while (is_.good());
            if (is_.eof()) is_.clear();
        }
    }
    template <typename T, bool> struct set_value {
        void operator () (std::string in, T &v) const {
            std::istringstream(in) >> v;
        }
    };
    template <typename T> struct set_value<T, true> {
        template <bool SIGNED> void convert (std::string in, T &v) const {
            if (SIGNED) v = ::strtoll(in.c_str(), 0, 0);
            else v = ::strtoull(in.c_str(), 0, 0);
        }
        void operator () (std::string in, T &v) const {
            convert<is_signed_int<T>::val>(in, v);
        }
    };
    template <typename T> const csv_istream & operator >> (T &v) const {
        std::string tmp;
        scan(&tmp);
        set_value<T, is_int<T>::val>()(tmp, v);
        return *this;
    }
    const csv_istream & operator >> (std::string &v) const {
        v.clear();
        scan_ws();
        if (is_.peek() != '"') scan(&v);
        else {
            std::string tmp;
            is_.get();
            std::getline(is_, tmp, '"');
            while (is_.peek() == '"') {
                v += tmp;
                v += is_.get();
                std::getline(is_, tmp, '"');
            }
            v += tmp;
            scan();
        }
        return *this;
    }
    template <typename T>
    const csv_istream & operator >> (T &(*manip)(T &)) const {
        is_ >> manip;
        return *this;
    }
    operator bool () const { return !is_.fail(); }
};


int main(int argc, char* argv[]) {
    PUNGraph Graph= TUNGraph::New();
    int i,max=0;
    int IsDir=0;
    char str[1024];
    int Length;
    if(argc != 6)
    {
            printf("not enough arguments, program exiting!");
            exit(0);
    }
    FILE *fp,*sp,*op1,*op2,*op3;
    fp= fopen(argv[1],"r");
    if(fp==NULL) printf("error");
    long int id1,id2;
    int amount;
    //building a graph with empty nodes defined by NNodes number
    for( i =0;i<NNodes;i++)
            Graph->AddNode(i);
    std::string title[5];
    std::string date, message;
    fgets(str,1024,fp);
    std::istringstream ss(str);
    csv_istream(ss)>> title[0] >> title[1] >> title[2] >>title[3]>>title[4];//processing the column headings
    //reading the file from the beginning and adding edges now from the batch payment file
    while(fgets(str,1024,fp)!=NULL)
    {
            std::istringstream ss(str);
            csv_istream(ss)>> date >> id1 >> id2 >>amount>>message;
            Graph->AddEdge(id1,id2);
    }
    //opening the stream file and processing it same as above
    sp = fopen(argv[2],"r");
    if(sp==NULL) printf("file read error");
    fgets(str,1024,sp); //reading in the column headings
    //opening the 3 output files in write mode
    op1 = fopen(argv[3],"w+");
    op2 = fopen(argv[4],"w+");
    op3 = fopen(argv[5],"w+");
    printf("Starting now\n");
    long int j=0;
    clock_t start;
    int flag,flag2;
    start = clock();
    while(fgets(str,1024,sp)!=NULL)
    {
            std::istringstream ss(str);
            csv_istream(ss)>> date >> id1 >> id2 >>amount>>message;
            flag = Graph->IsEdge(id1,id2);
            if(flag) Length=1;
            else Length = GetShortPath(Graph, id1, id2, IsDir);

            flag2=Graph->IsEdge(id1,id2);
            if(flag2)
                fprintf(op1,"trusted\n");
            else
                fprintf(op1,"unverified\n");

            if(Length>=0 && Length<=2)
                fprintf(op2,"trusted\n");
            else
                fprintf(op2,"unverified\n");

            if(Length>=0 && Length<=4)
                fprintf(op3,"trusted\n");
            else
                fprintf(op3,"unverified\n");
        j++;
        if( j % 10000 ==0) printf("lines processed:%ld\n",j);
    }
printf("Time elapsed: %f\n", ((double)clock() - start) / CLOCKS_PER_SEC);
 if(feof(sp))
        printf("end of stream file was reached\n");
 else printf("end of stream file was not reached, error!");

fclose(op1);
fclose(op2);
fclose(op3);
fclose(fp);
fclose(sp);


}


