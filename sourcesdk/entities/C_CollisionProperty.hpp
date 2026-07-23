//
// Created by sh1rm4n on 7/23/26.
//

#ifndef CENTURY_C_COLLISIONPROPERTY_HPP
#define CENTURY_C_COLLISIONPROPERTY_HPP

#include "../../core/utils.hpp"

class Vector;

class C_CollisionProperty {
public:
	//Don't forget the constructor.
	/*0*/	virtual Vector& GetEntityHandle(void) = 0;
	/*1*/	virtual Vector& OBBMinsPreScaled(void)const = 0;
	/*2*/	virtual Vector& OBBMaxsPreScaled(void)const = 0;
	/*3*/	virtual Vector& OBBMins(void)const = 0;
	/*4*/	virtual Vector& OBBMaxs(void)const = 0;
	/*5*/	virtual void WorldSpaceTriggerBounds(Vector* pVecWorldMins, Vector* pVecWorldMaxs)const = 0;
	/*6*/	virtual bool TestCollision(Ray const&, unsigned int mask, void*&) = 0;
	/*7*/	virtual bool TestHitboxes(Ray const&, unsigned int mask, void*&) = 0;
	/*8*/	virtual int GetCollisionModelIndex(void) = 0;
	/*9*/	virtual void* GetCollisionModel(void) = 0;
	/*10*/	virtual void* GetCollisionOrigin(void)const = 0;
	/*11*/	virtual void* GetCollisionAngles(void)const = 0;
	/*12*/	virtual void* CollisionToWorldTransform(void)const = 0;
	/*13*/	virtual void* GetSolid(void)const = 0;
	/*14*/	virtual void* GetSolidFlags(void)const = 0;
	/*15*/	virtual void* GetIClientUnknown(void) = 0;
	/*16*/	virtual void* GetCollisionGroup(void)const = 0;
	/*17*/	virtual void* WorldSpaceSurroundingBounds(Vector*, Vector*) = 0;
	/*18*/	virtual void* ShouldTouchTrigger(int)const = 0;
	/*19*/	virtual void* GetRootParentToWorldTransform(void)const = 0;
};

#endif //CENTURY_C_COLLISIONPROPERTY_HPP
