#include <hsdk/win/frame/direct3d/d3d10_meshanimation.h>



using namespace hsdk::direct3d;


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_MeshAnimation, void, clear)(
	/* [x] */ void)
{
	my_BoneOfNames.clear();
	my_Bones.clear();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_MeshAnimation, setup0)(
	/* [r] */ unsigned int _numOfBones,
	/* [r] */ unsigned int _numOfAnimation)
{
	if (my_Bones.size())
	{
		return E_ACCESSDENIED;
	}

	if (my_Animation.size())
	{
		return E_ACCESSDENIED;
	}

	my_Bones.resize(_numOfBones);
	my_Animation.resize(_numOfAnimation);

	return S_OK;;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_MeshAnimation, setup1_BoneNode)(
	/* [r] */ unsigned int _indexOfBone,
	/* [r] */ unsigned int _parentID,
	/* [r] */ unsigned int _lengthOfBone,
	/* [r] */ const wchar_t * _nameOfBone,
	/* [r] */ const float * _matrix)
{
	unsigned int bonesize = my_Bones.size();
	IF_FALSE(_indexOfBone < bonesize)
	{
		return E_INVALIDARG;
	}

	IF_FALSE(_parentID < bonesize)
	{
		return E_INVALIDARG;
	}

	D3D10MY_BONE & bone =
		my_Bones[_indexOfBone];

	bone.parentID = _parentID;
	bone.length = _lengthOfBone;
	bone.name = _nameOfBone;
	bone.id = _indexOfBone;
	memcpy(&bone.mRelation, _matrix, sizeof(D3DXMATRIX));

	my_BoneOfNames[bone.name] = _indexOfBone;

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_MeshAnimation, userSet_BoneMatrix)(
	/* [r] */ unsigned int _indexOfBone,
	/* [r] */ const float * _matrix)
{
	IF_FALSE(_indexOfBone < my_Bones.size())
	{
		return E_INVALIDARG;
	}

	memcpy(&my_Bones[_indexOfBone].mOwn, _matrix, sizeof(D3DXMATRIX));

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_MeshAnimation, setup1_Animation)(
	/* [r] */ unsigned int _indexOfAnimation,
	/* [r] */ const wchar_t * _nameOfAnimation,
	/* [r] */ unsigned int _numOfAnimateBones,
	/* [r] */ double _tickPerSecond,
	/* [r] */ double _secDuration)
{
	IF_FALSE(_indexOfAnimation < my_Animation.size())
	{
		return E_INVALIDARG;
	}

	D3D10MY_ANIMATION & animation =
		my_Animation[_indexOfAnimation];

	animation.name = _nameOfAnimation;
	animation.boneFrames.resize(_numOfAnimateBones);
	animation.tickPerSecond = _tickPerSecond;
	animation.secDuration = _secDuration;

	my_AnimationOfNames[animation.name] = _numOfAnimateBones;

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_MeshAnimation, setup2_AnimationBoneKeyFrame)(
	/* [r] */ unsigned int _indexOfAnimation,
	/* [r] */ unsigned int _indexOfAnimateBones,
	/* [r] */ unsigned int _boneID,
	/* [r] */ unsigned int _numOfAnimateFrameKeys,
	/* [r] */ const D3DXVECTOR3 * _postionStream,
	/* [r] */ const D3DXVECTOR3 * _rotationStream,
	/* [r] */ const D3DXVECTOR3 * _scalingStream,
	/* [r] */ D3D10MY_FRAME_HINT _frameHint)
{
	IF_FALSE(_indexOfAnimation < my_Animation.size())
	{
		return E_INVALIDARG;
	}

	IF_FALSE(_indexOfAnimateBones < my_Animation[_indexOfAnimation].boneFrames.size())
	{
		return E_INVALIDARG;
	}

	D3D10MY_ANIMATION_BONE_KEYFRAME & boneKeyFrame =
		my_Animation[_indexOfAnimation].boneFrames[_indexOfAnimateBones];

	boneKeyFrame.boneID = _boneID;
	boneKeyFrame.frameHint = _frameHint;
	boneKeyFrame.keyFrame.resize(_numOfAnimateFrameKeys);

	D3D10MY_ANIMATION_KEY * boneKeyBuffer = &boneKeyFrame.keyFrame[0];
	for (unsigned int index = 0; index < _numOfAnimateFrameKeys; ++index)
	{
		D3D10MY_ANIMATION_KEY & key = boneKeyBuffer[index];

		if (_postionStream)
		{
			memcpy(&key.position, &_postionStream[index], sizeof(D3DXVECTOR3));
		}

		if (_rotationStream)
		{
			memcpy(&key.rotation, &_rotationStream[index], sizeof(D3DXVECTOR3));
		}

		if (_scalingStream)
		{
			memcpy(&key.scaling, &_scalingStream[index], sizeof(D3DXVECTOR3));
		}
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC(D3D10_MeshAnimation, userSet_AnimationBoneKey)(
	/* [r] */ unsigned int _indexOfAnimation,
	/* [r] */ unsigned int _indexOfAnimateBones,
	/* [r] */ unsigned int _indexOfAnimateKey,
	/* [r] */ const D3DXVECTOR3 & _position,
	/* [r] */ const D3DXVECTOR3 & _rotation,
	/* [r] */ const D3DXVECTOR3 & _scaling)
{
	IF_FALSE(_indexOfAnimation < my_Animation.size())
	{
		return E_INVALIDARG;
	}

	IF_FALSE(_indexOfAnimateBones < my_Animation[_indexOfAnimation].boneFrames.size())
	{
		return E_INVALIDARG;
	}

	D3D10MY_ANIMATION_BONE_KEYFRAME & keyframe =
		my_Animation[_indexOfAnimation].boneFrames[_indexOfAnimateBones];

	IF_FALSE(_indexOfAnimateKey < keyframe.keyFrame.size())
	{
		return E_INVALIDARG;
	}

	D3D10MY_ANIMATION_KEY & key = keyframe.keyFrame[_indexOfAnimateBones];
	memcpy(&key.position, &_position, sizeof(D3DXVECTOR3));
	memcpy(&key.rotation, &_rotation, sizeof(D3DXVECTOR3));
	memcpy(&key.scaling, &_scaling, sizeof(D3DXVECTOR3));

	return S_OK;
}


//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_MeshAnimation, unsigned int, get_NumOfBones)(
	/* [x] */ void)const
{
	return my_Bones.size();
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_MeshAnimation, const D3D10MY_BONE *, find_BoneFromName)(
	/* [r] */ const wchar_t * _nameOfBone)const
{
	auto iter = my_BoneOfNames.find(_nameOfBone);
	if (iter == my_BoneOfNames.end())
	{
		return nullptr;
	}

	return &my_Bones[iter->second];
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_MeshAnimation, const D3D10MY_BONE *, find_BoneFromID)(
	/* [r] */  unsigned int _id)const
{
	IF_FALSE(_id < my_Bones.size())
	{
		return nullptr;
	}

	return &my_Bones[_id];
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_MeshAnimation, unsigned int, find_BoneIDFromName)(
	/* [r] */ const wchar_t * _nameOfBone)const
{
	auto iter = my_BoneOfNames.find(_nameOfBone);
	if (iter == my_BoneOfNames.end())
	{
		return -1;
	}

	return iter->second;
}

//--------------------------------------------------------------------------------------
CLASS_IMPL_FUNC_T(D3D10_MeshAnimation, void, transbone)(
	/* [w] */ D3DXMATRIX * _matrixbuffer,
	/* [w] */ unsigned int _numOfMatrixs,
	/* [w] */ const D3DXMATRIX _form)
{
	unsigned int limit = min(_numOfMatrixs, my_Bones.size());
	const D3D10MY_BONE * my_bones = &my_Bones[0];

	_matrixbuffer[0] = _form * my_bones[0].mOwn;
	for (unsigned int index = 1; index < limit; ++index)
	{
		const D3D10MY_BONE & bone = my_bones[index];

		_matrixbuffer[index] = _matrixbuffer[bone.parentID] * bone.mRelation;
	}

	for (unsigned int index = 1; index < limit; ++index)
	{
		_matrixbuffer[index] = _matrixbuffer[index] * my_bones[index].mOwn;
	}
}