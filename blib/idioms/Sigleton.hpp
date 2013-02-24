#ifndef __BLIB_SINGLETON_INCLUDED__
#define __BLIB_SINGLETON_INCLUDED__

namespace blib{namespace idioms{

   template<typename Type>
   class Singleton
   {
   public:
      Type& instance()
      {
         static Type retType;
         return retType;
      }
   private:
      Singleton();
      Singleton& operator=();
      Singleton(const Singleton&);
   };
}
}

#endif // __BLIB_SINGLETON_INCLUDED__