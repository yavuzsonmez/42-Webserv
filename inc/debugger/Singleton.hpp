#ifndef SINGLETON
# define SINGLETON

/**
 * A singleton is a design pattern that restricts the instantiation of a class to one object.
 * This is very useful, as example when you want to use the same Debugger instance everywhere.
 */
template<typename X>
class Singleton
{
    public:
        static X& getInstance()
        {
            static X    instance;
            return instance;
        }
        Singleton() {}
        Singleton(Singleton const&) {};
		~Singleton() {};
		Singleton & operator = (const Singleton &src);
    private:
 };

 #endif