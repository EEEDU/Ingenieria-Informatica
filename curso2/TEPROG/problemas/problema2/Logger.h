#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "Store.h"
#include "StoreMax.h"
#include "StoreMin.h"
#include "StoreLast.h"
#include "StoreAvg.h"

using namespace std;

template <typename T>
class Logger
{
    protected:
        string name;
        vector<Store<T>*> stores;

    public:
        Logger(const string& nam, const vector<Store<T>*> sto);
        ~Logger();
        
        void push(const T& dato);
        void push(const vector<T>& datos);
        void log() const;

};

template <typename T>
Logger<T>::Logger(const string& nam, const vector<Store<T>*> sto)
    : name(nam), stores(sto) {}

template <typename T>
Logger<T>::~Logger()
{
    for (Store<T>* store : stores)
    {
        delete store;   // libera memoria
    }
    stores.clear();     // opcional, limpia el vector
}

template <typename T>
void Logger<T>::push(const T& dato)
{
    for(Store<T>* store : stores)
    {
        store->push(dato);
    }
}

template <typename T>
void Logger<T>::push(const vector<T>& datos)
{
    for(Store<T>* store : stores)
    {
        store->push(datos);
    }
}

template <typename T>
void Logger<T>::log() const
{
    cout << name << ": " << endl;
    for(Store<T>* store : stores)
    {
        cout << "\tEl valor del almacen " << store->type() << " es " << store->value() << endl;
    }
}
