/**
 * A singleton is a design pattern that restricts the instantiation of a class to one object.
 * This is very useful, as example when you want to use the same Debugger instance everywhere.
 */
class Singleton
{
    public:
        static Singleton& getInstance()
        {
            static Singleton    instance;
            return instance;
        }
        Singleton() {}
        Singleton(Singleton const&) {};
		~Singleton() {};
		Singleton & operator = (const Singleton &src);
    private:
 };