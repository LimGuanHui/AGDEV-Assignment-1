#ifndef WOLF_H
#define WOLF_H

#include "../GenericEntity.h"
#include "../GroundEntity.h"

class Wolf : public GenericEntity
{
public:
    Wolf(void);
    virtual ~Wolf();

    void Init(void);
private:

};


#endif