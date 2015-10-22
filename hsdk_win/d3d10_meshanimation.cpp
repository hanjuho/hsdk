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

	my_Bones.resize(_numOfBones);

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
CLASS_IMPL_FUNC(D3D10_MeshAnimation, setup2_BoneMatrix)(
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

}