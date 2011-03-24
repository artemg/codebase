#include <map>
#include <algorithm>
#include <stdio.h>
#include <set>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <sys/stat.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>

using namespace std;

#define ALARM_TIME 10
#define NUM_EXP 11
static long next = 1;
volatile int work;
void start_profile();
double end_profile();

typedef long type1;
typedef int type2;

map<type1, type2>::iterator m_it;
set<type1>::iterator s_it;
vector<type1>::iterator v_it;

inline type1 my_rand(){
    return random();
}

void break_work(int x){
    work = 0;
}

long test_map_find(map<type1, type2> *m, long its){
    type1 finder;
    register long ret = 0;
    while(work){
        finder = my_rand();
        m_it = m->find(finder);
        ret++;
    }
    return ret;
}

long test_set_find(set<type1> *s, long its){
    type1 finder;
    register long ret = 0;
    while(work){
        finder = my_rand();
        s_it = s->find(finder);
        ret++;
    }
    return ret;
}

long test_vector_find(vector<type1> *v, long its){
    type1 finder;
    register long ret = 0;
    while(work){
        finder = my_rand();
        binary_search (v->begin(), v->end(), finder);
        ret++;
    }
    return ret;
}

void test_map_inserts(map<type1, type2> *m, type2 x, long its){
    type1 inserter;
    for(long i=0; i<its; i++){
        inserter = my_rand();
        (*m)[inserter] = x;
    }
}

void test_map_make_full(map<type1, type2> *m, type2 x, long its){
    type1 inserter;
    while(m->size() < its){
        inserter = my_rand();
        (*m)[inserter] = x;
    }
}

void test_set_inserts(set<type1> *s, long its){
    type1 inserter;
    for(long i=0; i<its; i++){
        inserter = my_rand();
        s->insert(inserter);
    }
}

void test_set_make_full(set<type1> *s, long its){
    type1 inserter;
    while(s->size() < its){
        inserter = my_rand();
        s->insert(inserter);
    }
}

void test_vector_push_back(vector<type1> *v, long its){
    type1 inserter;
    for(long i=0; i<its; i++){
        inserter = my_rand();
        v->push_back(inserter);
    }
}

void test_vector_sort(vector<type1> *v){
    sort(v->begin(), v->end());
}

template<class T>
T vmin(vector<T> &v){
    T min;
    for(typename vector<T>::iterator i = v.begin();
            i != v.end();
            i++)
    {
        if(*i < min || i == v.begin() )
            min = *i;
    }
    return min;
}
template<class T>
T vmax(vector<T> &v){
    T max;
    for(typename vector<T>::iterator i = v.begin();
            i != v.end();
            i++)
    {
        if(*i > max || i == v.begin() )
            max = *i;
    }
    return max;
}

template<class T>
T vavg(vector<T> &v) {
    assert(v.size() > 0);
    T sum = 0;
    for(typename vector<T>::iterator i = v.begin();
            i != v.end();
            i++)
    {
        sum += *i;
    }
    return sum / v.size();
}


int main(int argc, char *argv[]){
    if( argc < 3 ){
        printf("Bad params\n");
        return -1;
    }
    int action  = atoi(argv[1]);
    long its    = atol(argv[2]);
    signal(SIGALRM, &break_work);
    long retl;
    double retd;
    vector<double> fdata, f2data;
    vector<long> ldata;

    if( action == 1 ){
        for(int i=0; i<NUM_EXP; i++){
            map<type1, type2> m;
            start_profile();
            test_map_inserts(&m, 1, its);
            retd = end_profile();
            if(i!=0)
                fdata.push_back(retd);

            test_map_make_full(&m, 1, its);

            work = 1;
            alarm(ALARM_TIME);
            retl = test_map_find(&m, its);
            if(i!=0)
                ldata.push_back(retl);
        }
        printf("Map_insert,%f,%f,%f\n", vmin(fdata), vmax(fdata), vavg(fdata));
        printf("Map_find,%ld,%ld,%ld\n", vmin(ldata), vmax(ldata), vavg(ldata));
    }
    if( action == 2 ){
        for(int i=0; i<NUM_EXP; i++){
            set<type1> s;
            start_profile();
            test_set_inserts(&s, its);
            retd = end_profile();
            if( i != 0 )
                fdata.push_back(retd);

            test_set_make_full(&s, its);

            work = 1;
            alarm(ALARM_TIME);
            retl = test_set_find(&s, its);
            if( i!= 0 )
                ldata.push_back(retl);
        }
        printf("Set_insert,%f,%f,%f\n", vmin(fdata), vmax(fdata), vavg(fdata));
        printf("Set_find,%ld,%ld,%ld\n", vmin(ldata), vmax(ldata), vavg(ldata));
    }

    if( action == 3 ){
        for(int i=0; i<NUM_EXP; i++){
            vector<type1> v;
            start_profile();
            test_vector_push_back(&v, its);
            retd = end_profile();
            if( i != 0 )
                fdata.push_back(retd);

            start_profile();
            test_vector_sort(&v);
            retd = end_profile();
            if( i != 0 )
                f2data.push_back(retd);

            work = 1;
            alarm(ALARM_TIME);
            retl = test_vector_find(&v, its);
            if( i != 0 )
                ldata.push_back(retl);
        }
        printf("Vector_insert,%f,%f,%f\n", vmin(fdata), vmax(fdata), vavg(fdata));
        printf("Vector_sort,%f,%f,%f\n", vmin(f2data), vmax(f2data), vavg(f2data));
        printf("Vector_find,%ld,%ld,%ld\n", vmin(ldata), vmax(ldata), vavg(ldata));
    }

//    printf("Mapsize =%ld, Setsize=%ld, Vsize=%ld\n", (long)m.size(), (long)s.size(), (long)v.size());
    return 0;
}

static struct timeval t1,t2;

inline void start_profile(){
    gettimeofday(&t1, NULL);
}

inline double end_profile(){
    gettimeofday(&t2, NULL);
    double d1 = double(t1.tv_sec) + double(t1.tv_usec ) / 1000000.0;
    double d2 = double(t2.tv_sec) + double(t2.tv_usec ) / 1000000.0;
    //    printf("Elapsed time: %fs.\n", d2-d1);
    return d2-d1;
}

