# Creating a language file

Create a file. You should name it based on the language you're using. For example, American English would be `en_us.txt`

Add a key, separate it with an equals, and finally add the value.

Example:
```
//hello.world2=Hello, World?
hello.world=Hello, World!
```

Lines that start with `//` will be treated as comments

# Using a language file

Open a file, or get the contents of a file.

Call `BE_I18N_TranslateFromFile` if it's from a file, otherwise call `BE_I18N_Translate`

Pass in the key you want to translate.

Example:
```c++
FILE* file = ...;

BE_I18N_TranslateFromFile(file, "hello.world"); // => Hello, World!

char* buffer = ...;

BE_I18N_Translate(buffer, "hello.world2"); // => hello.world2
// "hello.world2" is commented out
```