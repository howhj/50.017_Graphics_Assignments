#ifndef GROUP_H
#define GROUP_H


#include "Object3D.h"
#include "Ray.h"
#include "Hit.h"
#include <iostream>

using  namespace std;


class Group:public Object3D
{
public:

    Group()
    {

    }
  	
    Group( int num_objects )
    {
       
    }

    ~Group()
    {
     
    }

    virtual bool intersect( const Ray& r , Hit& h , float tmin ) 
    {
       //return true only if theres at least 1 object in group is intersecting with the ray
       bool returnval = false; 
        
        for (int i = 0; i<objects.size(); i++)
        {
            Object3D* obj = objects[i];
            bool ans = obj->intersect(r , h, tmin);
            if (ans)
            {
                returnval = true;
            }
        }
        
        return returnval;
     }
  	
     void addObject( int index , Object3D* obj )
     {
         objects.push_back(obj);
     }

     int getGroupSize()
     { 
         return (int)objects.size();
     }


private:
    vector<Object3D*> objects;
};

#endif
	
