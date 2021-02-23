#include <gtest/gtest.h>
#include <service/abstract_object.hpp>


TEST(ABSTRACT_OBJECT, Create) {
    using namespace service;
    abstract_object document;

    auto* main_object = new value_object();

    {
        auto* screen = new value_object();

        {
            auto* title_value = new value_string("Title");
            auto* width_value = new value_integer(1980);
            auto* height_value = new value_integer(1080);

            screen->add("title", title_value);
            screen->add("width", width_value);
            screen->add("height", height_value);
        }

        main_object->add("screen", screen);

        auto* visible = new value_boolean(false);
        main_object->add("visible", visible);

        auto* something = new value_null();
        main_object->add("something", something);

        auto* getero_list = new value_list();

        {
            getero_list->add(new value_string("fuck"));
            getero_list->add(new value_integer(42));
            getero_list->add(new value_float(42.24));
            getero_list->add(new value_boolean(true));
            getero_list->add(new value_null());

            auto* in_list_object = new value_object();
            in_list_object->add("a", new value_string("AAAA"));
            in_list_object->add("b", new value_string("BBBB"));

            {
                auto* in_object_object = new value_object();
                in_object_object->add("int", new value_integer(199));
                in_object_object->add("null", new value_null());

                in_list_object->add("object", in_object_object);
            }

            getero_list->add(in_list_object);
        }

        main_object->add("Geteroheneous LIST", getero_list);

        {
            auto* in_object_object = new value_object();
            in_object_object->add("int", new value_integer(199));
            in_object_object->add("null", new value_null());

            main_object->add("object", in_object_object);
        }

        
        auto* empty_list = new value_list();
        main_object->add("Empty list", empty_list);
    }

    document.set(main_object);
    document.print();
}

