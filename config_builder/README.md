### EXAMPLE

    {
        // Note: this format supports comments.
        //   1. Object is the set of key-value pairs
        //   2. Key is have to be valid identifier i.e. satisfy this regex: ([\w_][\w\d_]*)
        //   3. Key and value are separated by equal sign '='
        //   4. Value can be:
        //      - double quoted string
        name = "Slavic";
        //      - integer number
        age = 28;
        //      - floating number
        height = 1.85;
        //      - boolean (true or false)
        human = true;
        //      - null value (null)
        dependencies = null;
        //      - objects
        window = {
            title  = "GL2";
            width  = 1980;
            height = 1080;
            mode = "window";
        }
        //      - lists
        characters = [
            {
                name = "Asuka"
                species = "Cat"
                height = 300
                fury = true
            }
            { 
                name = "Cherepaksa"
                species = "Ma'am"
                height = 228
                fury = false
            }
        ]
        // Note that lists are geterogenerous and can be nested
        // Also note that trailing semicolons are optional as well as commas in lists.
        GeteroList = [ null true false "foo" 1 2 3.4 {} [] [[]] ]
    }
