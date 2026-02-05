
#include <iostream>

class Cuenta
{
    protected:
        double capital_inicial;
        double interes;

    public:
        Cuenta(double cap_ini, double ini);
        ~Cuenta();
        double total(Cuenta cuentas[], int n, int t);
        double tae();
};

class CuentaCorriente : public Cuenta
{
    public:
        CuentaCorriente(double cap_ini, double ini);
        ~CuentaCorriente();
        double valor(int t) const;
};

class PlazoFijo : public Cuenta
{
    private:
        int plazo;

    public:
        PlazoFijo(double cap_ini, double ini, int pla);
        ~PlazoFijo();
        double valor(int t) const;
};

class CuentaNomina : public Cuenta
{
    private:
        double nomina;

    public:
        CuentaNomina(double cap_ini, double ini, double nom);
        ~CuentaNomina();
        double valor(int t) const;
};

class CuentaDivisa : public Cuenta
{
    public:
        CuentaDivisa(double cap_ini, double ini);
        ~CuentaDivisa();
        double valor(int t) const;
};
