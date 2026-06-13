//
// Created by sh1rm4n on 5/24/26.
//

#ifndef CENTURY_ICLIENTRENDERABLE_HPP
#define CENTURY_ICLIENTRENDERABLE_HPP

class Vector;
class Angle;
class Matrix3x4;

class IClientUnknown;

struct model_t
{
	void*	fnHandle;
	const char*			strName;

	int					nLoadFlags;		// mark loaded/not loaded
	int					nServerCount;	// marked at load
	void			**ppMaterials;	// null-terminated runtime material cache; ((intptr_t*)(ppMaterials))[-1] == nMaterials

	int			type;
	int					flags;			// MODELFLAG_???

	// volume occupied by the model graphics
	Vector				mins, maxs;
	float				radius;

	union
	{
		void*		brush;
		void*		studio;
		void*		sprite;
	};
};

class IClientRenderable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual Vector const& GetRenderOrigin() = 0;
	virtual Angle const& GetRenderAngles() = 0;
	virtual bool					ShouldDraw() = 0;
	virtual bool					IsTransparent() = 0;
	virtual bool					UsesPowerOfTwoFrameBufferTexture() = 0;
	virtual bool					UsesFullFrameBufferTexture() = 0;
	virtual void*					GetShadowHandle() = 0;
	virtual void*                   RenderHandle() = 0;
	virtual const model_t*          GetModel() = 0;
	virtual void					DrawModel(int flags) = 0;
	virtual int						GetBody() = 0;
	virtual void					ComputeFxBled() = 0;
	virtual int						GetFxBlend() = 0;
	virtual void					GetColorModulation(float* color) = 0;
	virtual bool					LODTest() = 0;
	virtual bool					SetupBones(Matrix3x4* pBoneToWorldOut, int nMaxBones, int boneMask, double currentTime) = 0;
	virtual void					SetupWeights(const Matrix3x4* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights) = 0;
	virtual void					DoAnimationEvents(void) = 0;
	virtual void* GetPVSNotifyInterface() = 0;
	virtual void					GetRenderBounds(Vector& mins, Vector& maxs) = 0;
	virtual void					GetRenderBoundsWorldspace(Vector& mins, Vector& maxs) = 0;
	virtual void					GetShadowRenderBounds(Vector& mins, Vector& maxs, void* shadowType) = 0;
	virtual bool					ShouldReceiveProjectedTextures(int flags) = 0;
	virtual bool					GetShadowCastDistance(float* pDist, void* shadowType) const = 0;
	virtual bool					GetShadowCastDirection(Vector* pDirection, void* shadowType) const = 0;
	virtual bool					IsShadowDirty() = 0;
	virtual void					MarkShadowDirty(bool bDirty) = 0;
	virtual IClientRenderable* GetShadowParent() = 0;
	virtual IClientRenderable* FirstShadowChild() = 0;
	virtual IClientRenderable* NextShadowPeer() = 0;
	virtual void*			ShadowCastType() = 0;
	virtual void					CreateModelInstance() = 0;
	virtual void*	GetModelInstance() = 0;
	virtual const Matrix3x4& RenderableToWorldTransform() = 0;
	virtual int						LookupAttachment(const char* pAttachmentName) = 0;
	virtual	bool					GetAttachment(int number, Vector& origin, Angle& angles) = 0;
	virtual bool					GetAttachment(int number, Matrix3x4& matrix) = 0;
	virtual float* GetRenderClipPlane(void) = 0;
	virtual int						GetSkin() = 0;
	virtual bool					IsTwoPass(void) = 0;
	virtual void					OnThreadedDrawSetup() = 0;
	virtual bool					UsesFlexDelayedWeights() = 0;
	virtual void					RecordToolMessage() = 0;
	virtual bool					IgnoresZBuffer(void) const = 0;
};

#endif //CENTURY_ICLIENTRENDERABLE_HPP
