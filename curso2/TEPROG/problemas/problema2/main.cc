#include "Store.h"
#include "StoreMax.h"
#include "StoreMin.h"
#include "StoreLast.h"
#include "StoreAvg.h"

#include "Logger.h"


#include <iostream>
#include <complex>
#include <vector>

using namespace std;

int main() {
    cout << "---------------------------------------------------------------------------------------" << endl;
    cout << "INT" << endl;
    cout << "---------------------------------------------------------------------------------------" << endl;

    Store<int>* storeMax = new StoreMax<int>();
    Store<int>* storeMin = new StoreMin<int>();
    Store<int>* storeAvg = new StoreAvg<int>();
    Store<int>* storeLast = new StoreLast<int>();

    storeMax->push(5);
    storeMax->push(8);
    storeMax->push(3);
    storeMax->push(7);
    storeMax->push({7, 5, 3});

    storeMin->push(5);
    storeMin->push(8);
    storeMin->push(3);
    storeMin->push(7);
    storeMin->push({7, 5, 3});

    storeAvg->push(5);
    storeAvg->push(8);
    storeAvg->push(3);
    storeAvg->push(7);
    storeAvg->push(9);
    storeAvg->push({7, 5, 3});

    storeLast->push(5);
    storeLast->push(8);
    storeLast->push(3);
    storeLast->push(7);
    storeLast->push({7, 5, 3});

    cout << "El valor del almacen max es: " << storeMax->value() << endl ; 
    cout << "El valor del almacen min es: " << storeMin->value() << endl ; 
    cout << "El valor del almacen avg es: " << storeAvg->value() << endl ; 
    cout << "El valor del almacen last es: " << storeLast->value() << endl ;
    
    cout << "---------------------------------------------------------------------------------------" << endl;
    cout << "FLOAT" << endl;
    cout << "---------------------------------------------------------------------------------------" << endl;

    Store<float>* storeMaxF = new StoreMax<float>();
    Store<float>* storeMinF = new StoreMin<float>();
    Store<float>* storeAvgF = new StoreAvg<float>();
    Store<float>* storeLastF = new StoreLast<float>();

    storeMaxF->push(5.2);
    storeMaxF->push(8.2);
    storeMaxF->push(3.2);
    storeMaxF->push(7.2);
    storeMaxF->push({7.2, 1.2, 5.6});

    storeMinF->push(5.2);
    storeMinF->push(8.2);
    storeMinF->push(3.2);
    storeMinF->push(7.2);
    storeMinF->push({7.2, 1.2, 5.6});

    storeAvgF->push(5.2);
    storeAvgF->push(8.2);
    storeAvgF->push(3.2);
    storeAvgF->push(7.2);
    storeAvgF->push(9.2);
    storeAvgF->push({7.2, 1.2, 5.6});

    storeLastF->push(5.2);
    storeLastF->push(8.2);
    storeLastF->push(3.2);
    storeLastF->push(7.2);
    storeLastF->push({7.2, 1.2, 5.6});


    cout << "El valor del almacen max es: " << storeMaxF->value() << endl ; 
    cout << "El valor del almacen min es: " << storeMinF->value() << endl ; 
    cout << "El valor del almacen avg es: " << storeAvgF->value() << endl ; 
    cout << "El valor del almacen last es: " << storeLastF->value() << endl ;

    cout << "---------------------------------------------------------------------------------------" << endl;
    cout << "COMPLEX<FLOAT>" << endl;
    cout << "---------------------------------------------------------------------------------------" << endl;

    Store<complex<float>>* storeMaxC = new StoreMax<std::complex<float>>();
    Store<complex<float>>* storeMinC = new StoreMin<std::complex<float>>();
    Store<complex<float>>* storeAvgC = new StoreAvg<std::complex<float>>();
    Store<complex<float>>* storeLastC = new StoreLast<std::complex<float>>();

    storeMaxC->push(complex<float>(2.5f, 1.0f));
    storeMaxC->push(complex<float>(3.5f, 1.0f));
    storeMaxC->push(complex<float>(2.5f, 5.0f));
    storeMaxC->push({ 
        complex<float>(2.5f, 5.0f),
        complex<float>(2.5f, 5.0f)
    });

    storeMinC->push(complex<float>(2.5f, 1.0f));
    storeMinC->push(complex<float>(3.5f, 1.0f));
    storeMinC->push(complex<float>(2.5f, 5.0f));
    storeMinC->push({ 
        complex<float>(2.5f, 5.0f),
        complex<float>(2.5f, 5.0f)
    });

    storeAvgC->push(complex<float>(2.5f, 1.0f));
    storeAvgC->push(complex<float>(3.5f, 1.0f));
    storeAvgC->push(complex<float>(2.5f, 5.0f));
    storeAvgC->push({ 
        complex<float>(2.5f, 5.0f),
        complex<float>(2.5f, 5.0f)
    });

    storeLastC->push(complex<float>(2.5f, 1.0f));
    storeLastC->push(complex<float>(2.5f, 5.0f));
    storeLastC->push(complex<float>(3.5f, 1.0f));
    storeLastC->push({ 
        complex<float>(2.5f, 5.0f),
        complex<float>(2.5f, 5.0f)
    });

    cout << "El valor del almacen max es: " << storeMaxC->value() << endl ; 
    cout << "El valor del almacen min es: " << storeMinC->value() << endl ; 
    cout << "El valor del almacen avg es: " << storeAvgC->value() << endl ; 
    cout << "El valor del almacen last es: " << storeLastC->value() << endl ; 

    cout << "---------------------------------------------------------------------------------------" << endl;
    cout << "VECTOR<DOUBLE>" << endl;
    cout << "---------------------------------------------------------------------------------------" << endl;

    Store<vector<double>>* storeMaxV = new StoreMax<vector<double>>();
    Store<vector<double>>* storeMinV = new StoreMin<vector<double>>();
    Store<vector<double>>* storeAvgV = new StoreAvg<vector<double>>();
    Store<vector<double>>* storeLastV = new StoreLast<vector<double>>();

    storeMaxV->push(vector<double>{1.0, 2.0, 3.0});
    storeMaxV->push(vector<double>{5.0, 2.0, 3.0});
    storeMaxV->push(vector<double>{1.0, 3.0, 3.0});
    storeMaxV->push({
        vector<double>{1.0, 3.0, 3.0},
        vector<double>{1.0, 3.0, 3.0},
    });

    storeMinV->push(vector<double>{1.0, 2.0, 3.0});
    storeMinV->push(vector<double>{5.0, 2.0, 3.0});
    storeMinV->push(vector<double>{1.0, 3.0, 3.0});
    storeMinV->push({
        vector<double>{1.0, 3.0, 3.0},
        vector<double>{1.0, 3.0, 3.0},
    });

    storeAvgV->push(vector<double>{1.0, 2.0, 1.0});
    storeAvgV->push(vector<double>{5.0, 2.0, 2.0});
    storeAvgV->push(vector<double>{1.0, 3.0, 3.0});
    storeAvgV->push({
        vector<double>{1.0, 3.0, 3.0},
        vector<double>{1.0, 3.0, 3.0},
    });
    
    storeLastV->push(vector<double>{1.0, 2.0, 3.0});
    storeLastV->push(vector<double>{5.0, 2.0, 3.0});
    storeLastV->push(vector<double>{1.0, 3.0, 3.0});
    storeLastV->push({
        vector<double>{1.0, 3.0, 3.0},
        vector<double>{1.0, 3.0, 3.0},
    });

    cout << "El valor del almacen max es: " << storeMaxV->value() << endl ; 
    cout << "El valor del almacen min es: " << storeMinV->value() << endl ; 
    cout << "El valor del almacen avg es: " << storeAvgV->value() << endl ; 
    cout << "El valor del almacen last es: " << storeLastV->value() << endl ; 

    delete storeMax, storeMin, storeAvg, storeLast;
    delete storeMaxF, storeMinF, storeAvgF, storeLastF;
    delete storeMaxC, storeMinC, storeAvgC, storeLastC;
    delete storeMaxV, storeMinV, storeAvgV, storeLastV;


    cout << "\n\n---------------------------------------------------------------------------------------" << endl;
    cout << "LOGGER" << endl;
    cout << "---------------------------------------------------------------------------------------" << endl;
    cout << "INT" << endl;
    cout << "---------------------------------------------------------------------------------------" << endl;

    Logger<int> temperature_log("temperature",
        {
            new StoreMax<int>,
            new StoreMin<int>,
            new StoreAvg<int>,
            new StoreLast<int>
        });
    temperature_log.push( 16 );
    temperature_log.push( {17, 25, 20} );
    temperature_log.log();

    cout << "---------------------------------------------------------------------------------------" << endl;
    cout << "FLOAT" << endl;
    cout << "---------------------------------------------------------------------------------------" << endl;

    Logger<float> temperatureF_log("temperature_float",
        {
            new StoreMax<float>,
            new StoreMin<float>,
            new StoreAvg<float>,
            new StoreLast<float>
        });
    temperatureF_log.push( 16.1 );
    temperatureF_log.push( {17.3, 25.4, 20.1} );
    temperatureF_log.log();


    cout << "---------------------------------------------------------------------------------------" << endl;
    cout << "COMPLEX<FLOAT>" << endl;
    cout << "---------------------------------------------------------------------------------------" << endl;

    Logger<complex<float>> temperatureC_log("temperature_complex",
        {
            new StoreMax<complex<float>>,
            new StoreMin<complex<float>>,
            new StoreAvg<complex<float>>,
            new StoreLast<complex<float>>
        });
    temperatureC_log.push( complex<float>(2.5f, 1.0f) );
    temperatureC_log.push( {
        complex<float>(4.5f, 1.0f), 
        complex<float>(1.5f, 10.0f), 
        complex<float>(3.5f, 1.0f)} );
    temperatureC_log.log();

    cout << "---------------------------------------------------------------------------------------" << endl;
    cout << "VECTOR<DOUBLE>" << endl;
    cout << "---------------------------------------------------------------------------------------" << endl;

    Logger<vector<double>> temperatureV_log("temperature_vector",
        {
            new StoreMax<vector<double>>,
            new StoreMin<vector<double>>,
            new StoreAvg<vector<double>>,
            new StoreLast<vector<double>>
        });
    temperatureV_log.push( vector<double>{1.0, 3.0, 1.0} );
    temperatureV_log.push( {
        vector<double>{1.0, 2.0, 2.0}, 
        vector<double>{1.0, 3.0}, 
        vector<double>{10.0, 3.0, 3.0}} );
    temperatureV_log.log();


    return 0;
}