// Hossein Moein
// March 18, 2018
// Copyright (C) 2018-2019 Hossein Moein
// Distributed under the BSD Software License (see file License)

#include <cstdlib>
#include <iostream>

#include <DateItem.h>
#include <ContainerItem.h>
#include <StopWatch.h>

using namespace hmdi;

// ----------------------------------------------------------------------------

int main (int argc, char *argv [])  {

    using namespace std;

    StopWatch             stop_watch;
    DMScu_FixedSizeString<1023> tstr;

    stop_watch.start ("FIRST");

    ContainerItem   conitem;
    ContainerItem   ci1;
    ContainerItem   ci2;
    ContainerItem   ci3;

    ci1.add_string_col ("userid", "moeinh");
    ci1.add_string_col ("cusip", "08000H9S6");
    ci1.add_int_col ("rank", 8);

    ci2.add_int_col ("id", 1);
    ci2.add_int_col ("modified_time", 932722339);
    ci2.add_string_col ("productType", "stock");
    ci2.add_string_col ("cusip", "0401149E2");
    ci2.add_string_col ("description", "ATT CORP of USA");
    ci2.add_string_col ("ccy", "USD");
    ci2.add_int_col ("issuerId", 41);
    ci2.add_int_col ("risk_1", 27);
    ci2.add_int_col ("test_int_column", 1);
    ci2.add_double_col ("notional", 1.000000);
    ci2.add_string_col ("ticker", "T.N");

    ci2.delete_reference ();  // should be a no-op

    cout << "----- Testing the *= (double) operator. You should get 45.34"
         << endl;
    ci2 ["notional"] *= 45.34;
    cout << "And the result is: "
         << ci2 ["notional"].to_double() << endl << endl;

    ci3.add_int_col ("id", 6);
    ci3.add_int_col ("modified_time", 0);
    ci3.add_string_col ("name", "ZAR");
    ci3.add_string_col ("description", "Sth Africa");
    ci3.add_string_col ("ccy", "JPY");
    ci3.add_string_col ("second_name", "none");

    stop_watch.stop ();
    std::cout << "Time Elapsed so far: "
              << stop_watch.elapsed_time (tstr)
              << std::endl << std::endl;

    conitem.add_double_col ("FirstDblCol", 4.5678);
    conitem.add_int_col ("FirstIntCol", 1024);
    conitem.add_string_col ("FirstStrCol", "This is a test column");
    conitem.add_col ("bfwd_clnt_config", ci1.clone ());
    conitem.add_double_col ("SecondDblCol", 1234.4563789234);
    conitem.add_col ("Stock", ci2.clone ());
    conitem.add_int_col ("SecondIntCol", 777);
    conitem.add_col ("third_container_item", ci3.clone ());
    conitem.add_longlong_col ("long_long_item", 12345678901234LL);
    conitem.add_col ("EMPTY_STRING", new StringItem);

    cout << "----- Testing ContainerItem Copy Constructor ..." << endl;

    ContainerItem   cp1 (conitem);
    ContainerItem   cp2 (ci1);
    ContainerItem   cp3 (ci2);
    ContainerItem   cp4 (ci3);

    if ((cp1 != conitem) || (cp2 != ci1) || (cp3 != ci2) | (cp4 != ci3))  {
        cout << "ERROR: ContainerItem's Copy Constructor doesn't "
                "produce identical items" << endl << endl;
        return (EXIT_FAILURE);
    }
    else  {
        cout << "SUCCESS: ContainerItem's copy constructor passed test"
             << endl << endl;
    }

    {
        cout << "----- Testing ContainerItem swap() Method ..." << endl;

        ContainerItem   copy_cont (conitem);
        ContainerItem   swapped_con;

        swapped_con.swap (copy_cont);
        if (swapped_con == conitem && copy_cont.num_cols () == 0 &&
            copy_cont.num_rows () == 0)  {
            cout << "SUCCESS: ContainerItem's swap method worked properly."
                 << endl << endl;
        }
        else  {
            cout << "ERROR: ContainerItem's swap method didn't work properly."
                 << endl;
            return (EXIT_FAILURE);
        }
    }

    try  {
        cout << "Number of columns: " << conitem.num_cols () << endl;
        cout << "Number of rows: " << conitem.num_rows () << endl;
//        cout << "Number of columns in bfwd_clnt_config: "
//             << conitem (0, "bfwd_clnt_config").num_cols () << endl;
//        cout << "Number of rows in bfwd_clnt_config: "
//             << conitem (0, "bfwd_clnt_config").num_rows () << endl;
        cout << "Number of columns in Bond: "
             << conitem (0, "Bond").num_cols () << endl;
        cout << "Number of rows in Bond: "
             << conitem (0, "Bond").num_rows () << endl;
        cout << "Number of columns in third_container_item: "
             << conitem (0, "third_container_item").num_cols () << endl;
        cout << "Number of rows in third_container_item: "
             << conitem (0, "third_container_item").num_rows () << endl;
        cout << endl << endl;

        conitem ["FirstDblCol"] = "4444.4444";
        conitem ["FirstStrCol"] = 123456.7890;

        cout << "----- Testing the [] operator ..." << endl;
        cout << "Row 0, Col FirstDblCol: "
             << conitem ["FirstDblCol"].to_double () << endl;
        cout << "Row 0, Col 0: "
             << conitem [static_cast<DataItem::size_type>(0)].to_double()
             << endl;
        cout << "Row 0, Col FirstIntCol: "
             << conitem ["FirstIntCol"].to_int () << endl;
        cout << "Row 0, Col 1: " << conitem[1].to_double () << endl;
        cout << "Row 0, Col FirstStrCol: "
             << conitem ["FirstStrCol"].to_stdstring () << endl;
        cout << "Row 0, Col SecondDblCol: "
             << conitem ["SecondDblCol"].to_double () << endl;
        cout << "Row 0, Col SecondIntCol: "
             << conitem ["SecondIntCol"].to_int () << endl;
        cout << "Row 0, Col long_long_item: "
             << conitem ["long_long_item"].to_longlong () << endl;
        cout << "Row 0, Col SecondIntCol via get (): "
             << conitem.get (0, "SecondIntCol")->to_int ()
             << endl;
        cout << '\n' << endl;

/*
        cout <<"Bond (3, issuerId): "
             << conitem (0, "Bond")(3, "issuerId").to_int ()
             << endl;
        cout <<"Bond (10, maturity): "
             << conitem (0, "Bond")(10, "maturity").to_int ()
             << endl;
        cout <<"Bond (10, description): "
             << conitem (0, "Bond")(10, "description").to_stdstring ()
             << endl;
        cout <<"Bond (12, cusip): "
             << conitem (0, "Bond")(12, "cusip").to_stdstring ()
             << endl;
        cout << '\n' << endl;
*/

        // NOTE: we may want to exploit return-value covariance for the "get"
        //       functions in the future...

       // WARNING: static downcast
       //
        const   ContainerItem   &ci_ref = static_cast<const ContainerItem &>
            (conitem (0, "third_container_item"));
/*
        cout << "third_container_item(12, description): "
             << ci_ref(12, "description").to_stdstring() << endl;
*/
        cout << "third_container_item(4, daysToSettle): "
             << ci_ref(4, "daysToSettle").to_int() << endl;
        cout << "third_container_item(ci_ref.num_rows () - 1, name): "
             << ci_ref(ci_ref.num_rows() - 1, "name").to_stdstring()
             << endl;

        cout << endl << "----- Testing some error checking ..." << endl;
        cout << "third_container_item(12, badname): "
             << ci_ref(12, "badname").to_stdstring() << endl;
        cout << "third_container_item(9999999, description): "
             << ci_ref(9999999, "description").to_stdstring() << endl;
        cout << "third_container_item(12, badname)<int>: "
             << ci_ref(12, "badname").to_int() << endl;
        cout << "third_container_item(9999999, description)<int>: "
             << ci_ref(9999999, "description").to_int() << endl;

       conitem ["badname"] = "test string";

       NullItem    nullval;

        if (ci_ref (12, "badname") == nullval)  {
            cout << "ERROR: ci_ref (12, badname) should not be equal to "
                    "nullval"
                 << endl;
            return (EXIT_FAILURE);
        }
        if (ci_ref (12, "badname") == ContainerItem::NullValue)  {
            cout << "ci_ref (12, badname) == ContainerItem::NullValue"
                 << endl;
        }
    }
    catch (const std::runtime_error &ex)  {
        cout << ex.what();
    }

    stop_watch.start ("SECOND");
    cout << endl << endl;
    ShowC (&conitem);
    cout << conitem;
    stop_watch.stop ();
    std::cout << "Time Elapsed so far: "
              << stop_watch.elapsed_time (tstr)
              << std::endl << std::endl;

//  cout << conitem (0, "Bond") << endl;
//  cout << conitem (0, "third_container_item") << endl;

    try  {
        if (conitem ["Bond"] == conitem ["third_container_item"])  {
            cout << "ERROR: conitem ['Bond'] == conitem "
                    "['third_container_item']"
                 << endl;
            return (EXIT_FAILURE);
        }
        if (conitem ["Bond"] == conitem ["Bond"])
            cout << "conitem ['Bond'] == conitem ['Bond']" << endl;
        if (conitem ["third_container_item"] ==
            conitem ["third_container_item"])
            cout << "conitem ['third_container_item'] == "
                    "conitem ['third_container_item']"
                 << endl;
    }
    catch (const std::runtime_error &ex)  {
        cout << ex.what();
    }

    cout << endl << "Binary size of container is: "
         << conitem.bin_size () << endl;

    unsigned    char    *buffer = new unsigned char [conitem.bin_size ()];

    try  {
        stop_watch.start ();

        conitem.bin_copy (buffer);

        ContainerItem   bin_cpy_con;

        bin_cpy_con.bin_construct (buffer);

        delete[] buffer;

        cout << "----- Comparing the binary constructed with original" << endl;
        if (conitem == bin_cpy_con)  {
            cout << "SUCCESS: conitem == bin_cpy_con" << endl << endl;
        }
        else  {
            cout << "ERROR: conitem != bin_cpy_con" << endl;
            return (EXIT_FAILURE);
        }
        cout << endl << endl;

        cout << bin_cpy_con;

        cout << endl << endl << "----- Checking the column remove method"
             << endl;
        bin_cpy_con.remove_col ("bfwd_clnt_config");
        cout << bin_cpy_con;

        cout << endl << endl << "----- Testing the project() method" << endl;
        cout << "Befor project:" << endl;
        stop_watch.stop ();
        std::cout << "Time Elapsed so far: "
                  << stop_watch.elapsed_time (tstr)
                  << std::endl << std::endl;
    }
    catch (const std::runtime_error &ex)  {
        cout << ex.what();
    }

    cout << ci2 << endl;
    cout << ci3 << endl;

    cout << "After project:" << endl;

    ci2 << ci3;
    cout << ci2 << endl;
    cout << ci3 << endl;

    cout << "----- Testing DateItem" << endl;

    stop_watch.start ("NEXT ONE");
    DateItem    now;
    DateItem    gmnow (DateItem::GMT);

    cout << "Local Time is: " << now.format_value (DT_TM2)
         << "   Nanoseconds: " << now.nanosec () << endl;
    cout << "GMT Time is: " << gmnow.format_value (DT_TM2)
         << "   Microseconds: " << now.microsec () << endl;
    cout << "Fame date is: " << now.format_value (DT_FAME_DATE) << endl;

    now = 19721202;
    cout << "Date/Time should be 19721202: "
         << now.format_value (DT_TM2) << endl;
    now = "19910223 16:23:45.230";
    cout << "Date/Time should be '19910223 16:23:45.230': "
         << now.format_value (DT_TM2) << endl;
    now = "19910223 16:23:45.230";
    cout << "Date/Time should be '19910223 16:23:45.230': "
         << now.format_value (DT_TM2) << endl;
    now = "20100207 12";
    cout << "Date/Time should be '20100207 12:00:00.000': "
         << now.format_value (DT_TM2) << endl;
    now = "20100207 12:32";
    cout << "Date/Time should be '20100207 12:32:00.000': "
         << now.format_value (DT_TM2) << endl;
    now = "20100207 12:32:12";
    cout << "Date/Time should be '20100207 12:32:12.000': "
         << now.to_stdstring () << endl;

    try  {
        long  diff = static_cast<long> (now.diff_seconds (gmnow));

        cout << "The difference between local and GMT times is:  "
             << diff / 3600;
        diff %= 3600;
        cout << ":" << diff / 60;
        diff %= 60;
        cout << ":" << diff << endl;
    }
    catch (const std::runtime_error &ex)  {
        cout << ex.what();
    }

    cout << "\n----- Testing ReferenceItem\n" << endl;

    ReferenceItem   ri (&ci2);

    cout << ri << endl;
    cout << "Number of columns: " << ri.num_cols() << endl;
    cout << "description: " << ri["description"].to_stdstring() << endl;

    ri ["modified_time"] = 20010618;
    cout << "modified_time (20010618): "
         << ri["modified_time"].to_int() << endl;

    cout << "\n----- Now assiging ci3 to the ReferenceItem\n" << endl;
    ri.set (ci3);
    cout << ri << endl;
    stop_watch.stop ();
    std::cout << "Time Elapsed so far: "
              << stop_watch.elapsed_time (tstr)
              << std::endl << std::endl;

    DataItem    &di = ri.get_refed_di ();

    cout << "\n----- Testing ReferenceItem's get_refed_di()\n" << endl;
    cout << di << endl;

//    ri.delete_reference ();

    cout << "\n----- Testing DateItem's add_days()\n" << endl;

    {
        DateItem    now;

        cout << "Now is: " << now.format_value (DT_TM2) << endl;
        now.add_days (1);
        cout << "After adding 1 day: " << now.format_value (DT_TM2) << endl;
        now.add_days (-1);
        cout << "After adding -1 day: " << now.format_value (DT_TM2) << endl;

        now.add_business_days (1);
        cout << "After adding 1 business day: "
             << now.format_value (DT_TM2) << endl;
        now.add_business_days (-1);
        cout << "After adding -1 business day: "
             << now.format_value (DT_TM2) << endl;

        {
            DateItem    di (20091224, 9, 30);


            cout << "After adding 2 business days to '"
                 << di.to_stdstring() << "' ";
            di.add_business_days (2);
            cout << "you get '" << di.to_stdstring() << "'" << endl;

        }

        {
            DateItem    di (20081224, 9, 30);


            cout << "After adding 2 business days to '"
                 << di.to_stdstring() << "' ";
            di.add_business_days (2);
            cout << "you get '" << di.to_stdstring() << "'" << endl;

        }

        {
            DateItem    di (20061224, 9, 30);


            cout << "After adding 2 business days to '"
                 << di.to_stdstring() << "' ";
            di.add_business_days (2);
            cout << "you get '" << di.to_stdstring() << "'" << endl;

        }

        {
            DateItem    di (20051223, 9, 30);


            cout << "After adding 2 business days to '"
                 << di.to_stdstring() << "' ";
            di.add_business_days (2);
            cout << "you get '" << di.to_stdstring() << "'" << endl;

        }

        {
            DateItem    di (20041223, 9, 30);


            cout << "After adding 2 business days to '"
                 << di.to_stdstring() << "' ";
            di.add_business_days (2);
            cout << "you get '" << di.to_stdstring() << "'" << endl;

        }

        {
            DateItem    di (20001222, 9, 30);


            cout << "After adding 2 business days to '"
                 << di.to_stdstring() << "' ";
            di.add_business_days (2);
            cout << "you get '" << di.to_stdstring() << "'" << endl;

        }

        {
            DateItem    di (19981224, 9, 30);


            cout << "After adding 2 business days to '"
                 << di.to_stdstring() << "' ";
            di.add_business_days (2);
            cout << "you get '" << di.to_stdstring() << "'" << endl;

        }
    }

    cout << "\n----- Testing DateItem's GMT vs Local time\n" << endl;

    {
        DateItem    gmt_now (DateItem::GMT);
        DateItem    local_now;

        DateItem    local_alt_con (local_now.to_int (), local_now.hour (),
                                   local_now.minute (), local_now.sec (),
                                   local_now.msec ());
        DateItem    gmt_alt_con (gmt_now.to_int (), gmt_now.hour (),
                                 gmt_now.minute (), gmt_now.sec (),
                                 gmt_now.msec (), DateItem::GMT);

        DateItem    local_time_1970 (19700101);
        DateItem    gmt_time_1970 (19700101, 0, 0, 0, 0, DateItem::GMT);

        DateItem    local_time_1989 (19891214, 20, 15, 23, 0);
        DateItem    gmt_time_1989 (19891214, 20, 15, 23, 0, DateItem::GMT);

        cout << "GMT now time: " << gmt_now.format_value (DT_TM2)
             << "   " << gmt_now.time () << endl;
        cout << "GMT ALT Constructor time: "
             << gmt_alt_con.format_value (DT_TM2)
             << "   " << gmt_alt_con.time () << endl << endl;

        cout << "Local now time: " << local_now.format_value (DT_TM2)
             << "   " << local_now.time ()  << endl;
        cout << "Local ALT Constructor time: "
             << local_alt_con.format_value (DT_TM2)
             << "   " << local_alt_con.time () << endl << endl;

        cout << "Local 1970 ALT Constructor time: "
             << local_time_1970.format_value (DT_TM2)
             << "   " << local_time_1970.time () << endl;
        cout << "GMT 1970 ALT Constructor time: "
             << gmt_time_1970.format_value (DT_TM2)
             << "   " << gmt_time_1970.time () << endl << endl;

        cout << "Local 1989 ALT Constructor time: "
             << local_time_1989.format_value (DT_TM2)
             << "   " << local_time_1989.time () << endl;
        cout << "GMT 1989 ALT Constructor time: "
             << gmt_time_1989.format_value (DT_TM2)
             << "   " << gmt_time_1989.time () << endl << endl;
    }

    cout << "\n----- Testing DateItem's business day methods\n" << endl;

    {
        const   DateItem    new_year (20020101);

        cout << "20020101 is a US holiday: "
             << (new_year.is_us_business_day () ? "NO" : "YES") << endl;

        const   DateItem    thanksgiving (20011122);

        cout << "20011122 (Thanksgiving) is a US holiday: "
             << (thanksgiving.is_us_business_day () ? "NO" : "YES") << endl;

        DateItem            yes_answers [80];
        int                 i = 0;
        const   DateItem    hol01 (19980101);  yes_answers [i++] = hol01;
        const   DateItem    hol02 (19980119);  yes_answers [i++] = hol02;
        const   DateItem    hol03 (19980216);  yes_answers [i++] = hol03;
        const   DateItem    hol04 (19980525);  yes_answers [i++] = hol04;
        const   DateItem    hol05 (19980703);  yes_answers [i++] = hol05;
        const   DateItem    hol06 (19980907);  yes_answers [i++] = hol06;
        const   DateItem    hol07 (19981126);  yes_answers [i++] = hol07;
        const   DateItem    hol08 (19981225);  yes_answers [i++] = hol08;
        const   DateItem    hol09 (19990101);  yes_answers [i++] = hol09;
        const   DateItem    hol10 (19990118);  yes_answers [i++] = hol10;
        const   DateItem    hol11 (19990215);  yes_answers [i++] = hol11;
        const   DateItem    hol12 (19990531);  yes_answers [i++] = hol12;
        const   DateItem    hol13 (19990705);  yes_answers [i++] = hol13;
        const   DateItem    hol14 (19990906);  yes_answers [i++] = hol14;
        const   DateItem    hol15 (19991125);  yes_answers [i++] = hol15;
        const   DateItem    hol16 (19991224);  yes_answers [i++] = hol16;
        const   DateItem    hol17 (19991231);  yes_answers [i++] = hol17;
        const   DateItem    hol18 (20000117);  yes_answers [i++] = hol18;
        const   DateItem    hol19 (20000221);  yes_answers [i++] = hol19;
        const   DateItem    hol20 (20000529);  yes_answers [i++] = hol20;
        const   DateItem    hol21 (20000704);  yes_answers [i++] = hol21;
        const   DateItem    hol22 (20000904);  yes_answers [i++] = hol22;
        const   DateItem    hol23 (20001123);  yes_answers [i++] = hol23;
        const   DateItem    hol24 (20001225);  yes_answers [i++] = hol24;
        const   DateItem    hol25 (20010101);  yes_answers [i++] = hol25;
        const   DateItem    hol26 (20010115);  yes_answers [i++] = hol26;
        const   DateItem    hol27 (20010219);  yes_answers [i++] = hol27;
        const   DateItem    hol28 (20010528);  yes_answers [i++] = hol28;
        const   DateItem    hol29 (20010704);  yes_answers [i++] = hol29;
        const   DateItem    hol30 (20010903);  yes_answers [i++] = hol30;
        const   DateItem    hol31 (20011122);  yes_answers [i++] = hol31;
        const   DateItem    hol32 (20011225);  yes_answers [i++] = hol32;
        const   DateItem    hol33 (20020101);  yes_answers [i++] = hol33;
        const   DateItem    hol34 (20020121);  yes_answers [i++] = hol34;
        const   DateItem    hol35 (20020218);  yes_answers [i++] = hol35;
        const   DateItem    hol36 (20020527);  yes_answers [i++] = hol36;
        const   DateItem    hol37 (20020704);  yes_answers [i++] = hol37;
        const   DateItem    hol38 (20020902);  yes_answers [i++] = hol38;
        const   DateItem    hol39 (20021128);  yes_answers [i++] = hol39;
        const   DateItem    hol40 (20021225);  yes_answers [i++] = hol40;
        const   DateItem    hol41 (20030101);  yes_answers [i++] = hol41;
        const   DateItem    hol42 (20030120);  yes_answers [i++] = hol42;
        const   DateItem    hol43 (20030217);  yes_answers [i++] = hol43;
        const   DateItem    hol44 (20030526);  yes_answers [i++] = hol44;
        const   DateItem    hol45 (20030704);  yes_answers [i++] = hol45;
        const   DateItem    hol46 (20030901);  yes_answers [i++] = hol46;
        const   DateItem    hol47 (20031127);  yes_answers [i++] = hol47;
        const   DateItem    hol48 (20031225);  yes_answers [i++] = hol48;
        const   DateItem    hol49 (20040101);  yes_answers [i++] = hol49;
        const   DateItem    hol50 (20040119);  yes_answers [i++] = hol50;
        const   DateItem    hol51 (20040216);  yes_answers [i++] = hol51;
        const   DateItem    hol52 (20040531);  yes_answers [i++] = hol52;
        const   DateItem    hol53 (20040705);  yes_answers [i++] = hol53;
        const   DateItem    hol54 (20040906);  yes_answers [i++] = hol54;
        const   DateItem    hol55 (20041125);  yes_answers [i++] = hol55;
        const   DateItem    hol56 (20041224);  yes_answers [i++] = hol56;
        const   DateItem    hol57 (20041231);  yes_answers [i++] = hol57;
        const   DateItem    hol58 (20050117);  yes_answers [i++] = hol58;
        const   DateItem    hol59 (20050221);  yes_answers [i++] = hol59;
        const   DateItem    hol60 (20050530);  yes_answers [i++] = hol60;
        const   DateItem    hol61 (20050704);  yes_answers [i++] = hol61;
        const   DateItem    hol62 (20050905);  yes_answers [i++] = hol62;
        const   DateItem    hol63 (20051124);  yes_answers [i++] = hol63;
        const   DateItem    hol64 (20051226);  yes_answers [i++] = hol64;
        const   DateItem    hol65 (20060102);  yes_answers [i++] = hol65;
        const   DateItem    hol66 (20060116);  yes_answers [i++] = hol66;
        const   DateItem    hol67 (20060220);  yes_answers [i++] = hol67;
        const   DateItem    hol68 (20060529);  yes_answers [i++] = hol68;
        const   DateItem    hol69 (20060704);  yes_answers [i++] = hol69;
        const   DateItem    hol70 (20060904);  yes_answers [i++] = hol70;
        const   DateItem    hol71 (20061123);  yes_answers [i++] = hol71;
        const   DateItem    hol72 (20061225);  yes_answers [i++] = hol72;
        const   DateItem    hol73 (20070101);  yes_answers [i++] = hol73;
        const   DateItem    hol74 (20070115);  yes_answers [i++] = hol74;
        const   DateItem    hol75 (20070219);  yes_answers [i++] = hol75;
        const   DateItem    hol76 (20070528);  yes_answers [i++] = hol76;
        const   DateItem    hol77 (20070704);  yes_answers [i++] = hol77;
        const   DateItem    hol78 (20070903);  yes_answers [i++] = hol78;
        const   DateItem    hol79 (20071122);  yes_answers [i++] = hol79;
        const   DateItem    hol80 (20071225);  yes_answers [i++] = hol80;

        for (i = 0; i < 80; i++)  {
            cout << (yes_answers [i].is_us_business_day ()
                        ?  (yes_answers [i].to_stdstring () +
                           " should be a holiday\n")
                        : "");
        }

        for (i = 0; i < 80; i++)  {
            for (int j = 0; j < 7; j++)  {
                yes_answers [i].add_days (1);
                if (! yes_answers [i].is_weekend () &&
                    ! yes_answers [i].is_us_business_day ())  {
                    cout << yes_answers [i].to_stdstring ()
                         << " may not be a holiday" << endl;
                }
            }
            yes_answers [i].add_days (-7);
            if (yes_answers [i].is_us_business_day ())  {
                cout << yes_answers [i].to_stdstring ()
                     << " should be a holiday" << endl;
            }
            for (int j = 0; j < 7; j++)  {
                yes_answers [i].add_days (-1);
                if (! yes_answers [i].is_weekend () &&
                    ! yes_answers [i].is_us_business_day ())  {
                    cout << yes_answers [i].to_stdstring ()
                         << " may not be a holiday" << endl;
                }
            }
        }

        i = 0;

        DateItem            columbus [10];
        const   DateItem    columbus01 (19981012);  columbus[i++] = columbus01;
        const   DateItem    columbus02 (19991011);  columbus[i++] = columbus02;
        const   DateItem    columbus03 (20001009);  columbus[i++] = columbus03;
        const   DateItem    columbus04 (20011008);  columbus[i++] = columbus04;
        const   DateItem    columbus05 (20021014);  columbus[i++] = columbus05;
        const   DateItem    columbus06 (20031013);  columbus[i++] = columbus06;
        const   DateItem    columbus07 (20041011);  columbus[i++] = columbus07;
        const   DateItem    columbus08 (20051010);  columbus[i++] = columbus08;
        const   DateItem    columbus09 (20061009);  columbus[i++] = columbus09;
        const   DateItem    columbus10 (20071008);  columbus[i++] = columbus10;

        for (i = 0; i < 10; i++)  {
            cout << (columbus [i].is_us_bank_holiday ()
                        ? ""
                        : (columbus [i].to_stdstring () +
                          " should be a bank holiday\n"));
            cout << (columbus [i].is_us_business_day ()
                        ? ""
                        : (columbus [i].to_stdstring () +
                          " should not be a regular holiday\n"));
        }
        for (i = 0; i < 10; i++)  {
            for (int j = 0; j < 7; j++)  {
                columbus [i].add_days (1);
                if (! columbus [i].is_weekend () &&
                    columbus [i].is_us_bank_holiday ())  {
                    cout << columbus [i].to_stdstring ()
                         << " should not be a bank holiday" << endl;
                }
                if (! columbus [i].is_weekend () &&
                    ! columbus [i].is_us_business_day ())  {
                    cout << columbus [i].to_stdstring ()
                         << " may not be a holiday" << endl;
                }
            }
            columbus [i].add_days (-7);
            if (! columbus [i].is_us_bank_holiday ())  {
                cout << columbus [i].to_stdstring ()
                     << " should be a regular holiday" << endl;
            }
            if (! columbus [i].is_us_business_day ())  {
                cout << columbus [i].to_stdstring ()
                     << " should not be a regular holiday" << endl;
            }
            for (int j = 0; j < 7; j++)  {
                columbus [i].add_days (-1);
                if (! columbus [i].is_weekend () &&
                    columbus [i].is_us_bank_holiday ())  {
                    cout << columbus [i].to_stdstring ()
                         << " should not be a bank holiday" << endl;
                }
                if (! columbus [i].is_weekend () &&
                    ! columbus [i].is_us_business_day ())  {
                    cout << columbus [i].to_stdstring ()
                         << " may not be a holiday" << endl;
                }
            }
        }
    }

    cout << "\n----- Testing DateItem's diff methods\n" << endl;
    stop_watch.start ("DIFF methods");

    {
        const   DateItem    di1 (20020101, 12, 54, 31);
        const   DateItem    di2 (20020201, 3, 50, 30);

        cout << "di1 is: " << di1.format_value (DT_TM2) << endl;
        cout << "di2 is: " << di2.format_value (DT_TM2) << endl;
        cout << "second difference between di1 and di2: "
             << di1.diff_seconds (di2) << endl;
        cout << "minute difference between di1 and di2: "
             << di1.diff_minutes (di2) << endl;
        cout << "hour difference between di1 and di2: "
             << di1.diff_hours (di2) << endl;
        cout << "day difference between di1 and di2: "
             << di1.diff_days (di2) << endl;
        cout << "business day difference between di1 and di2: "
             << di1.diff_business_days (di2) << endl;
        cout << "week difference between di1 and di2: "
             << di1.diff_weeks (di2) << endl;

        size_t          index = 0;
        const   size_t  max_index = 10000;
        DateItem        now_performance (19700101);

        while (++index < max_index)  {
            now_performance.add_business_days (1);
        }

        cout << "After adding " << max_index << " business days to 19700101, "
             << "the date is " << now_performance.format_value (DT_TM2)
             << endl;
    }

    stop_watch.stop ();
    std::cout << "Time Elapsed so far: "
              << stop_watch.elapsed_time (tstr)
              << std::endl << std::endl;

    {
        cout << "\n----- Testing Empty StringItem's\n" << endl;

        StringItem  str;

        cout << "Empty string is: \"" << str << "\"\n";

        StringItem  str2 ("This is test string");

        cout << "Test string is: \"" << str2 << "\"\n";

        str2 = str;
        cout << "Test string after empty string assignment \""
             << str2 << "\"\n" << endl;
    }

    {
        cout << "\n----- Testing empty ContainerItem assignment\n" << endl;

        ContainerItem   empty_container;
        ContainerItem   copy_container (empty_container);

        cout << "SUCCESS: Empty contianer was copied properly" << endl << endl;
    }

    cout << "\n----- Testing add_row()\n" << endl;
    {
        try  {
            ContainerItem conitem;

            conitem.add_col ("One", new IntItem (3));
            conitem.add_col ("Two", new IntItem (3));

            int i = 0;

            for (int row = 0; row < 5; row++)  {
                conitem.add_row ();
                for (int col = 0; col < 2; col++)  {
                    conitem (row, col) = i;
                    i++;
                }
            }

            cout << conitem.num_rows () << endl;
            cout << conitem << endl;
        }
        catch (const std::runtime_error& e)  {
            cerr << e.what();
        }
    }

    cout << "\n----- Testing DateItem's DST vs EST (Going Forward)\n" << endl;
    {
        {
            DateItem    start (20060331);
            DateItem    test;
            int         i = 0;

            cout << "\tGoing from EST to DST\n" << endl;
            while (i++ <= 10)  {
                test = start;
                cout << test.format_value (DT_TM2)
                     << " --- " << test.time () << endl;
                start.add_days (1);
            }
        }

        {
            DateItem    start (20051028);
            DateItem    test;
            int         i = 0;

            cout << endl << endl;
            cout << "\tGoing from DST to EST\n" << endl;
            while (i++ <= 10)  {
                test = start;
                cout << test.format_value (DT_TM2)
                     << " --- " << test.time () << endl;
                start.add_days (1);
            }
        }

        {
            DateItem    start (20060331);

            cout << endl << endl;
            cout << "\tGoing from EST to DST in one shot\n" << endl;

            cout << start.format_value (DT_TM2)
                 << " --- " << start.time () << endl;
            start.add_days (10);
            cout << start.format_value (DT_TM2)
                 << " --- " << start.time () << endl;
        }

        {
            DateItem    start (20051028);

            cout << endl << endl;
            cout << "\tGoing from DST to EST in one shot\n" << endl;

            cout << start.format_value (DT_TM2)
                 << " --- " << start.time () << endl;
            start.add_days (10);
            cout << start.format_value (DT_TM2)
                 << " --- " << start.time () << endl;
        }
    }


    cout << "\n----- Testing DateItem's DST vs EST (Going Backward)\n" << endl;

    {
        {
            DateItem    start (20060408);
            DateItem    test;
            int         i = 0;

            cout << "\tGoing from EST to DST\n" << endl;
            while (i++ <= 10)  {
                test = start;
                cout << test.format_value (DT_TM2)
                     << " --- " << test.time () << endl;
                start.add_days (-1);
            }
        }

        {
            DateItem    start (20051105);
            DateItem    test;
            int         i = 0;

            cout << endl << endl;
            cout << "\tGoing from DST to EST\n" << endl;
            while (i++ <= 10)  {
                test = start;
                cout << test.format_value (DT_TM2)
                     << " --- " << test.time () << endl;
                start.add_days (-1);
            }
        }

        {
            DateItem    start (20060408);

            cout << endl << endl;
            cout << "\tGoing from EST to DST in one shot\n" << endl;

            cout << start.format_value (DT_TM2)
                 << " --- " << start.time () << endl;
            start.add_days (-10);
            cout << start.format_value (DT_TM2)
                 << " --- " << start.time () << endl;
        }

        {
            DateItem    start (20051105);

            cout << endl << endl;
            cout << "\tGoing from DST to EST in one shot\n" << endl;

            cout << start.format_value (DT_TM2)
                 << " --- " << start.time () << endl;
            start.add_days (-10);
            cout << start.format_value (DT_TM2)
                 << " --- " << start.time () << endl;
        }

        {
            DateItem    start (20090309, 16, 30);

            cout << endl << endl;
            cout << "\tGoing from EST to DST in one shot (2)\n" << endl;

            cout << start.format_value (DT_TM2)
                 << " --- " << start.time () << endl;
            start.add_days (-20);
            cout << start.format_value (DT_TM2)
                 << " --- " << start.time () << endl;
        }

        {
            DateItem    start (20081103, 16, 30);

            cout << endl << endl;
            cout << "\tGoing from DST to EST in one shot (3)\n" << endl;

            cout << start.format_value (DT_TM2)
                 << " --- " << start.time () << endl;
            start.add_business_days (-1);
            cout << start.format_value (DT_TM2)
                 << " --- " << start.time () << endl;
        }
    }

    cout << "\n----- Testing DateItem's going over year boundary\n" << endl;

    {
        {
            DateItem    test (20081225, 16, 30);
            int         i = 0;

            cout << endl << endl;
            cout << "\tGoing Forward\n" << endl;

            while (i++ <= 10)  {
                cout << test.format_value (DT_TM2)
                     << " --- " << test.time () << endl;
                test.add_days (1);
            }
        }
    }

    {
        {
            DateItem    test (20080105, 16, 30);
            int         i = 0;

            cout << endl << endl;
            cout << "\tGoing Backward\n" << endl;

            while (i++ <= 10)  {
                cout << test.format_value (DT_TM2)
                     << " --- " << test.time () << endl;
                test.add_days (-1);
            }
        }
    }

    cout << "\n----- Testing DateItem's time zone changes\n" << endl;

    {
        DateItem    local_now;
        DateItem    gmt_now (DateItem::GMT);
        DateItem    b_aires_now (DateItem::AM_BUENOS_AIRES);
        DateItem    tehran_now (DateItem::AS_TEHRAN);
        DateItem    tokyo_now (DateItem::AS_TOKYO);
        DateItem    sydney_now (DateItem::AU_SYDNEY);
        DateItem    nz_now (DateItem::NZ);
        DateItem    moscow_now (DateItem::EU_MOSCOW);
        DateItem    shang_now (DateItem::AS_SHANGHAI);
        DateItem    berlin_now (DateItem::EU_BERLIN);
        DateItem    paris_now (DateItem::EU_PARIS);
        DateItem    stock_now (DateItem::EU_STOCKHOLM);
        DateItem    ny_now (DateItem::AM_NEW_YORK);
        DateItem    la_now (DateItem::AM_LOS_ANGELES);
        DateItem    local_now_2;

        cout << "Now Local:        " << local_now << endl;
        cout << "GMT Local:        " << gmt_now << endl;
        cout << "Now Buenos Aires: " << b_aires_now << endl;
        cout << "Now Tehran:       " << tehran_now << endl;
        cout << "Now Tokyo:        " << tokyo_now << endl;
        cout << "Now Sydeny:       " << sydney_now << endl;
        cout << "Now New Zealand:  " << nz_now << endl;
        cout << "Now Moscow:       " << moscow_now << endl;
        cout << "Now Shanghai:     " << shang_now << endl;
        cout << "Now Berlin:       " << berlin_now << endl;
        cout << "Now Paris:        " << paris_now << endl;
        cout << "Now Stockholm:    " << stock_now << endl;
        cout << "Now New York:     " << ny_now << endl;
        cout << "Now Los Amgeles:  " << la_now << endl;
        cout << "Now Local (2):    " << local_now_2 << endl;

        local_now.set_timezone (DateItem::AU_SYDNEY);
        cout << "\nGoing from Local to Sydney: " << local_now << endl;

        DateItem    local (20091123, 9, 38, 43, 345);

        cout << "\nLocal Date/Time: " << local << endl;
        local.set_timezone (DateItem::AU_SYDNEY);
        cout << "\nLocal changed to Sydney: " << local << endl;
    }

    cout << "\n----- Testing DateItem's const char * constructor\n" << endl;

    {
         DateItem   di1 ("20100207 12:31");

         if (di1.to_stdstring () != "02/07/2010 12:31:00.000")  {
             std::cout << "ERROR: " << di1.to_stdstring () << " != "
                       << "02/07/2010 12:31:00.000" << std::endl;
             return (EXIT_FAILURE);
         }

         DateItem   di2 ("02/08/2010 12:31:56", DateItem::AME_STYLE);

         if (di2.to_stdstring () != "02/08/2010 12:31:56.000")  {
             std::cout << "ERROR: " << di2.to_stdstring () << " != "
                       << "02/08/2010 12:31:56.000" << std::endl;
             return (EXIT_FAILURE);
         }

         DateItem   di3 ("2010/02/09 12:31:56", DateItem::EUR_STYLE);

         if (di3.to_stdstring () != "02/09/2010 12:31:56.000")  {
             std::cout << "ERROR: " << di3.to_stdstring () << " != "
                       << "02/09/2010 12:31:56.000" << std::endl;
             return (EXIT_FAILURE);
         }

         DateItem   di4 ("  2010/02/10 12:31:56", DateItem::EUR_STYLE);

         if (di4.to_stdstring () != "02/10/2010 12:31:56.000")  {
             std::cout << "ERROR: " << di4.to_stdstring () << " != "
                       << "02/10/2010 12:31:56.000" << std::endl;
             return (EXIT_FAILURE);
         }

         DateItem   di5 ("    02/11/2010 12:31:56", DateItem::AME_STYLE);

         if (di5.to_stdstring () != "02/11/2010 12:31:56.000")  {
             std::cout << "ERROR: " << di5.to_stdstring () << " != "
                       << "02/11/2010 12:31:56.000" << std::endl;
             return (EXIT_FAILURE);
         }
    }

    std::cout << "\n\nTotal Time Elapsed: "
              << stop_watch.total_elapsed_time (tstr)
              << std::endl << std::endl;

    std::cout << "\n\nAll Elapsed Times:\n"
              << stop_watch.all_elapsed_times (tstr)
              << std::endl << std::endl;

    return (EXIT_SUCCESS);
}

// ----------------------------------------------------------------------------

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
