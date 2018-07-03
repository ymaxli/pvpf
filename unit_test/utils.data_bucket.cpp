//
// Created by Max on 2018/7/1.
//
#ifdef STAND_ALONE
#define BOOST_TEST_MODULE DataBucket
#endif

#include <boost/test/unit_test.hpp>
#include <pvpf/utils/data_bucket.hpp>
#include <iostream>
#include <core/any.hpp>

using namespace std;
using namespace pvpf;

int constructor_count = 0;
int destructor_count = 0;
int copy_count = 0;
int move_count = 0;

class A {
public:
    bool output;
    string id;
    int content;

    A(bool out, string &&id = "a", int content = 0) :
            output(out), id(id), content(content) {
        constructor_count++;
        print("constructor");
    }

    ~A() {
        destructor_count++;
        print("destructor");
    }

    A(const A &a) : output(a.output), id(a.id + ".copy"), content(a.content) {
        copy_count++;
        print("copy constructor");
    }

    A(A &&a) : output(a.output), id(a.id + ".move"), content(a.content) {
        move_count++;
        print("move constructor");
    }

private:

    void print(string &&str) {
        if (output) {
            cout << id + " " + str << endl;
        }
    }
};

void reset_counters() {
    constructor_count = 0;
    destructor_count = 0;
    copy_count = 0;
    move_count = 0;
}

BOOST_AUTO_TEST_SUITE(utils_data_bucket_suite)

    BOOST_AUTO_TEST_CASE(copy_put) {
        reset_counters();

        {
            A a(false);
            data_bucket bucket;

            bucket.put("abc", a);
        }

        BOOST_TEST(constructor_count + copy_count + move_count == destructor_count);
        BOOST_TEST(constructor_count == 1);
        BOOST_TEST(move_count == 0);
        BOOST_TEST(copy_count == 1);
    }

    BOOST_AUTO_TEST_CASE(move_put) {
        reset_counters();

        {
            A a(false);
            data_bucket bucket;

            bucket.put("abc", std::move(a));
        }

        BOOST_TEST(constructor_count + copy_count + move_count == destructor_count);
        BOOST_TEST(constructor_count == 1);
        BOOST_TEST(move_count == 1);
        BOOST_TEST(copy_count == 0);
    }

    BOOST_AUTO_TEST_CASE(copy_override_an_entry) {
        reset_counters();

        {
            A a(false, "a");
            A b(false, "b");
            data_bucket bucket;

            bucket.put("abc", a);
            bucket.put("abc", b);
        }

        BOOST_TEST(constructor_count + copy_count + move_count == destructor_count);
        BOOST_TEST(constructor_count == 2);
        BOOST_TEST(move_count == 0);
        BOOST_TEST(copy_count == 2);
    }

    BOOST_AUTO_TEST_CASE(move_override_an_entry) {
        reset_counters();

        {
            A *a = new A(false, "a");
            A b(false, "b");
            data_bucket bucket;

            bucket.put("abc", std::move(*a));
            bucket.put("abc", std::move(b));
            delete a;
        }

        BOOST_TEST(constructor_count + copy_count + move_count == destructor_count);
        BOOST_TEST(constructor_count == 2);
        BOOST_TEST(move_count == 2);
        BOOST_TEST(copy_count == 0);
    }

    BOOST_AUTO_TEST_CASE(move_maintain_life_time) {
        reset_counters();

        {
            data_bucket bucket;
            {
                A b(false, "b", 100);

                bucket.put("abc", std::move(b));
            }

            BOOST_TEST(constructor_count == 1);
            BOOST_TEST(move_count == 1);
            BOOST_TEST(copy_count == 0);
            BOOST_TEST(destructor_count == 1);

            A *a = bucket.get_ptr<A>("abc");
            BOOST_TEST(a->content == 100);
        }

        BOOST_TEST(constructor_count + copy_count + move_count == destructor_count);
    }

    BOOST_AUTO_TEST_CASE(move_get_then_put_back) {
        reset_counters();

        {
            data_bucket bucket;
            A b(false, "b", 100);

            bucket.put("abc", std::move(b));

            A *a = bucket.get_ptr<A>("abc");
            BOOST_TEST(a->content == 100);
        }

        BOOST_TEST(constructor_count + copy_count + move_count == destructor_count);
        BOOST_TEST(copy_count == 0);
    }

    BOOST_AUTO_TEST_CASE(copy_get) {
        reset_counters();

        {
            data_bucket bucket;
            A b(false, "b", 100);

            bucket.put("abc", std::move(b));

            A a = bucket.get_copy<A>("abc");
            BOOST_TEST(a.content == 100);
        }

        BOOST_TEST(constructor_count + copy_count + move_count == destructor_count);
    }

    BOOST_AUTO_TEST_CASE(get_primitive_type) {
        reset_counters();

        {
            data_bucket bucket;

            bucket.put("abc", 1.2f);

            auto a = bucket.get_copy<float>("abc");
            BOOST_TEST(a == 1.2f);
        }

        BOOST_TEST(constructor_count + copy_count + move_count == destructor_count);
    }

    BOOST_AUTO_TEST_CASE(move_assign_to_self) {
        reset_counters();

        {
            data_bucket bucket;
            bucket.put("abc", 1.2f);
            bucket = std::move(bucket);

            auto a = bucket.get_copy<float>("abc");
            BOOST_TEST(a == 1.2f);
        }
    }

    BOOST_AUTO_TEST_CASE(get_copy_exception) {
        reset_counters();

        {
            data_bucket bucket;
            bucket.put("abc", 1.2f);

            try {
                bucket.get_copy<float>("ac");
                BOOST_TEST(false);
            } catch (std::exception& e) {
                BOOST_TEST(!string("key:ac does not exist").compare(e.what()));
            }
        }
    }

    BOOST_AUTO_TEST_CASE(get_ptr_exception) {
        reset_counters();

        {
            data_bucket bucket;
            bucket.put("abc", 1.2f);

            try {
                bucket.get_ptr<float>("acd");
                BOOST_TEST(false);
            } catch (std::exception& e) {
                BOOST_TEST(!string("key:acd does not exist").compare(e.what()));
            }
        }
    }

BOOST_AUTO_TEST_SUITE_END()
