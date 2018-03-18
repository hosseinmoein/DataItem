// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#include <cmath>
#include <cstdlib>
#include <iostream>

#include <SystemItem.h>

using namespace hmdi;

//-----------------------------------------------------------------------------

// This is an example of a specific system.
// I know that what it does doesn't make sense, however
// it shows how a specific system can be designed.
//
class   BondSystem: public SystemItem  {

    public:

        BondSystem () : SystemItem (2, 1)  {

            _construct ();
            _wireUp ();
        }
        BondSystem (const BondSystem &ot) : SystemItem (ot)  {   }
        BondSystem (const ContainerItem &con) : SystemItem (con)  {

            _wireUp ();
        }

    protected:

        DataItem *_clone_hook () const  { return new BondSystem (*this); }
        void _construct ();
        void _wireUp ();

    private:

        EXEC_RES price2yield (const DataItem &price,
                              DataItem &yield,
                              const void *user_data) const;
        EXEC_RES yield2price (const DataItem &yield,
                              DataItem &price,
                              const void *user_data) const;
        EXEC_RES settl2yield (const DataItem &settl,
                              DataItem &yield,
                              const void *user_data) const;
        EXEC_RES settl2ai (const DataItem &settl,
                           DataItem &yield,
                           const void *user_data) const;

        EXEC_RES TestCi2price (const DataItem &test_ci,
                               DataItem &price,
                               const void *user_data) const;
        EXEC_RES TestCi2yield (const DataItem &test_ci,
                               DataItem &yield,
                               const void *user_data) const;

        EXEC_RES priceActn (const DataItem &price,
                            const void *user_data) const;
        EXEC_RES yieldActn (const DataItem &yield,
                            const void *user_data) const;
        EXEC_RES aiActn (const DataItem &ai,
                         const void *user_data) const;
        EXEC_RES settlActn (const DataItem &settl,
                            const void *user_data) const;

        EXEC_RES price2repeat_col (const DataItem &price,
                                   DataItem &repeat_col,
                                   const void *user_data) const;
};

//-----------------------------------------------------------------------------

void BondSystem::_construct ()  {

    add_double_col ("price", 101.234);
    add_double_col ("yield", 5.46);
    add_double_col ("ai", 0.4563);
    add_double_col ("coupon", 5.0);
    add_int_col ("AsOfDate", 19991123);
    add_int_col ("settlement", 19991223);

/*
    for (int i = 0; i < 3000; ++i)  {
        DMScu_FixedSizeString<63>   str;

        str.printf ("TestCol%d", i);
        add_double_col (str.c_str (), 5.0);
    }
*/

    ContainerItem   *ci = new ContainerItem;

    add_col ("TEST_CI", ci);

    ci->add_double_col ("col_1", 10);
    ci->add_double_col ("col_2", 1);

    return;
}

//-----------------------------------------------------------------------------

void BondSystem::_wireUp ()  {

    add_dependency ("price", "yield", (DpndcyMethod) &BondSystem::price2yield);
    add_dependency ("yield", "price", (DpndcyMethod) &BondSystem::yield2price);
    add_dependency ("settlement", "yield",
                    (DpndcyMethod) &BondSystem::settl2yield);
    add_dependency ("settlement", "ai", (DpndcyMethod) &BondSystem::settl2ai);

    add_dependency ("TEST_CI", "price",
                    (DpndcyMethod) &BondSystem::TestCi2price);
    add_dependency ("TEST_CI", "yield",
                    (DpndcyMethod) &BondSystem::TestCi2yield);

/*
    for (int i = 0; i < 3000; ++i)  {
        DMScu_FixedSizeString<63>   str;

        str.printf ("TestCol%d", i);
        add_dependency ("price",
                        str.c_str (),
                        (DpndcyMethod) &BondSystem::price2repeat_col);
    }
*/

    ContainerItem   *ci = new ContainerItem;
    add_action ("price", (ActionMethod) &BondSystem::priceActn);
    add_action ("yield", (ActionMethod) &BondSystem::yieldActn);
    add_action ("ai", (ActionMethod) &BondSystem::aiActn);
    add_action ("settlement", (ActionMethod) &BondSystem::settlActn);

    return;
}

//-----------------------------------------------------------------------------

SystemItem::EXEC_RES BondSystem::price2yield (const DataItem &price,
                                              DataItem &yield,
                                              const void *user_data) const  {

    using namespace std;

    yield = ((*this)["coupon"].to_double() /
             price.to_double()) * 100.0;
    cout << "Price --> Yield Dependency was called: "
         << price.to_double() << ", "
         << yield.to_double() << endl;

    return (_success_);
}

//-----------------------------------------------------------------------------

SystemItem::EXEC_RES BondSystem::
price2repeat_col (const DataItem &price,
                  DataItem &repeat_col,
                  const void *user_data) const  {

    std::cout << "Printing from the repeat col dependency function"
         << std::endl;

    return (_success_);
}

//-----------------------------------------------------------------------------

SystemItem::EXEC_RES BondSystem::yield2price (const DataItem &yield,
                                              DataItem &price,
                                              const void *user_data) const  {

    using namespace std;

    double  prc = 0.0;

    for (int i = 0; i <= 20; ++i)  {
        double  df = 1.0 / pow (1.0 + (*this) ["yield"].to_double () /
                                    100.0 / (i + 1.0),
                                (i * 30.0 / 365.0) * (i + 1.0));

        prc += df;
    }
    prc /= 10000.0;

    prc = prc + static_cast<double> ((*this) ["ai"]);
    price = prc * 1000.0 / 4.0;
    cout << "Yield --> Price Dependency was called: "
         << static_cast<double> (yield) << ", "
         << static_cast<double> (price) << endl;

    return (_success_);
}

//-----------------------------------------------------------------------------

SystemItem::EXEC_RES BondSystem::settl2ai (const DataItem &settl,
                                           DataItem &ai,
                                           const void *user_data) const  {

    using namespace std;

    ai = static_cast<double>((*this) ["coupon"]) *
         (static_cast<double> (settl) /
          static_cast<double> ((*this) ["AsOfDate"])) +
         static_cast<double> ((*this) ["coupon"]) *
         (static_cast<double> (settl) - 5.0) /
         static_cast<double> (settl);
    cout << "Settlement --> Accrud Dependency was called: "
         << static_cast<int> (settl) << ", "
         << static_cast<double> (ai) << endl;

    return (_success_);
}

//-----------------------------------------------------------------------------

SystemItem::EXEC_RES BondSystem::settl2yield (const DataItem &settl,
                                              DataItem &yield,
                                              const void *user_data) const  {
    using namespace std;

    yield = static_cast<double> (settl) -
            static_cast<double> ((*this) ["price"]) / 100000.0;
    cout << "Settlement --> Yield Dependency was called: "
         << static_cast<int> (settl) << ", "
         << static_cast<double> (yield) << endl;

    return (_success_);
}

//-----------------------------------------------------------------------------

SystemItem::EXEC_RES BondSystem::TestCi2price (const DataItem &test_ci,
                                               DataItem &price,
                                               const void *user_data) const  {

    std::cout << "TestCi --> Price was called:\n"
              << test_ci << "\n"
              << price.to_double () << "\n\n" << std::endl;

    return (_success_);
}

//-----------------------------------------------------------------------------

SystemItem::EXEC_RES BondSystem::TestCi2yield (const DataItem &test_ci,
                                               DataItem &yield,
                                               const void *user_data) const  {

    std::cout << "TestCi --> Yield was called:\n"
              << test_ci << "\n"
              << yield.to_double () << "\n\n" << std::endl;

    return (_success_);
}

//-----------------------------------------------------------------------------

SystemItem::EXEC_RES BondSystem::priceActn (const DataItem &price,
                                            const void *user_data) const  {
    using namespace std;

    cout << "Price Action was called: " << static_cast<double> (price) << endl;
    return (_success_);
}

//-----------------------------------------------------------------------------

SystemItem::EXEC_RES BondSystem::yieldActn (const DataItem &yield,
                                            const void *user_data) const  {
    using namespace std;

    cout << "Yield Action was called: " << static_cast<double> (yield) << endl;
    return (_success_);
}

//-----------------------------------------------------------------------------

SystemItem::EXEC_RES BondSystem::aiActn (const DataItem &ai,
                                         const void *user_data) const  {
    using namespace std;

    cout << "Accrued Action was called: " << static_cast<double> (ai) << endl;
    return (_success_);
}

//-----------------------------------------------------------------------------

SystemItem::EXEC_RES BondSystem::settlActn (const DataItem &settl,
                                            const void *user_data) const  {
    using namespace std;

    cout << "Settlement Action was called: "
         << static_cast<int> (settl) << endl;
    return (_success_);
}

//-----------------------------------------------------------------------------

int main (int argc, char *argv [])  {

    using namespace std;

    BondSystem  bndsys;

    // bndsys.add_dependency ("price", "yield",
    //                        (SystemItem::DpndcyMethod) bndsys.price2yield);

    bndsys ["price"] = 100.01;
    // cout << bndsys << endl << endl;
    cout << endl;

    bndsys ["yield"] = 5.0;
    // cout << bndsys << endl << endl;
    cout << endl;

    bndsys ["settlement"] = 19991224;
    // cout << bndsys << endl << endl;
    cout << endl;

    BondSystem  bndsys2 (bndsys);

    cout << "\tTESTING THE COPY CONSTRUCTOR:" << endl << bndsys2 << endl;

    // bndsys2.dpndcyOff ();

    bndsys2 ["price"] = 200;
    // cout << bndsys2 << endl << endl;
    cout << endl;

    // bndsys2.rmvDependency ("yield", "price");

    bndsys2 ["yield"] = 5.5;
    // cout << bndsys2 << endl << endl;
    cout << endl;

   // Now testing the dependencies on container item columns
   //
    bndsys2 ["TEST_CI"] ["col_1"] = 20.0;
    bndsys2 ["TEST_CI"] ["col_2"] = 2.0;

    ContainerItem   ci;

    ci.add_double_col ("col_1", 30);
    ci.add_double_col ("col_2", 3);
    bndsys2 ["TEST_CI"] = ci;

#ifdef LATER
    cout << "\tTESTING THE CONTAINER CONSTRUCTOR:" << endl << endl;

    ContainerItem   itm;

    itm.add_double_col ("price", 101.234);
    itm.add_double_col ("yield", 5.46);
    itm.add_double_col ("ai", 0.4563);
    itm.add_double_col ("coupon", 5.0);
    itm.add_int_col ("AsOfDate", 19991123);
    itm.add_int_col ("settlement", 19991223);

    BondSystem  bndsys3 (itm);

    cout << bndsys3 << endl;

    bndsys3 ["price"] = 200;
    cout << bndsys3 << endl << endl;

    bndsys3 ["yield"] = 5.5;
    cout << bndsys3 << endl << endl;

    bndsys3 ["settlement"] = 19991229;
    cout << bndsys3 << endl << endl;
#endif // LATER

    return (EXIT_SUCCESS);
}

//-----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
