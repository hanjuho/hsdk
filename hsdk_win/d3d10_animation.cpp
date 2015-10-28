#include <hsdk/win/direct3d/d3d10_animation.h>


//--------------------------------------------------------------------------------------
DLL_DECL_FUNC_T(void, hsdk::direct3d::animation::animationClear)(
	_Out_ D3D10_Animation & _animation)
{
	_animation.animationPath.clear();
	_animation.animations.clear();

	_animation.bonePath.clear();
	_animation.bones.clear();
}

//--------------------------------------------------------------------------------------
DLL_DECL_FUNC_T(void, hsdk::direct3d::animation::animationRecordClear)(
	_Out_ D3D10_Animation_Recorder & _recorder)
{
	_recorder.aniamtionID = -1;
	_recorder.keyframe.clear();
	_recorder.pos.clear();
	_recorder.time = 0.0f;
}

//--------------------------------------------------------------------------------------
DLL_DECL_FUNC_T(void, hsdk::direct3d::animation::build_MeshBoneMatrix)(
	_Out_ D3DXMATRIX * _matrixbuffer,
	_In_ const unsigned int * _boneNodeBuffer,
	_In_ unsigned int _bufferSize,
	_In_ const D3D10_Animation & _animation,
	_In_ const D3D10_Animation_Recorder & _pos)
{
	const D3D10MY_BONE * dispatchBone = &_animation.bones[0];
	const D3DXMATRIX * dispatchPos = &_pos.pos[0];
	for (unsigned int index = 0; index < _bufferSize; ++index)
	{
		const unsigned int boneID = _boneNodeBuffer[index];
		_matrixbuffer[index] = dispatchPos[boneID] * dispatchBone[boneID].mOwn;
		D3DXMatrixTranspose(&_matrixbuffer[index], &_matrixbuffer[index]);
	}
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(unsigned int, hsdk::direct3d::animation::create_Pos)(
	_Out_ D3D10_Animation_Recorder & _pos,
	_In_ const D3D10_Animation & _animation,
	_In_ unsigned int _animationPos,
	_In_ double _time)
{
	const D3D10MY_BONE * boneDispatch = &_animation.bones[0];

	unsigned int limit = _animation.bones.size();
	if (_pos.pos.size() < limit)
	{
		_pos.pos.resize(limit);
	}

	D3DXMATRIX * dispatch = &_pos.pos[0];
	D3DXMatrixIdentity(&dispatch[0]);

	bool animateBone[256] = { 0 };
	{
		const D3D10MY_ANIMATION & animation = _animation.animations[_animationPos];
		const unsigned int length = animation.boneKeyFrames.size();

		{
			_pos.keyframe.resize(length);
			memset(&_pos.keyframe[0], -1, sizeof(D3D10MY_KEYFRAME_RECORD)* length);
		}

		for (unsigned int index = 0; index < length; ++index)
		{
			const D3D10MY_BONE_KEYFRAME & boneKeyFrame = animation.boneKeyFrames[index];
			D3D10MY_KEYFRAME_RECORD & refrecord = _pos.keyframe[index];

			dispatch[boneKeyFrame.boneID] = compute_MatrixFromBoneKeyFrame(
				boneKeyFrame, animation.duration, _time, &refrecord);

			animateBone[boneKeyFrame.boneID] = true;
			refrecord.boneID = boneKeyFrame.boneID;
		}
	}

	for (unsigned int index = 0; index < limit; ++index)
	{
		const D3D10MY_BONE & bone = boneDispatch[index];

		if (animateBone[index])
		{
			dispatch[index] = dispatch[bone.parent] * dispatch[index];
		}
		else
		{
			dispatch[index] = dispatch[bone.parent] * bone.mRelation;
		}
	}

	_pos.aniamtionID = _animationPos;

	return limit;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(D3DXMATRIX, hsdk::direct3d::animation::compute_MatrixFromBoneKeyFrame)(
	_In_ const D3D10MY_BONE_KEYFRAME & _boneKeyframe,
	_In_ double _duration,
	_In_ double _time,
	_Out_opt_ D3D10MY_KEYFRAME_RECORD * _keyFrame)
{
	aiVector3D position;
	aiQuaternion rotation;
	aiVector3D scale;

	const aiAnimBehaviour preFrameHint = _boneKeyframe.preFrameMoveHint;
	const aiAnimBehaviour postFrameHint = _boneKeyframe.postFrameMoveHint;

	if (_keyFrame)
	{
		position = compute_VectorFromKeyFrame<aiVectorKey, aiVector3D>(
			_boneKeyframe.positionKeyFrame,
			preFrameHint, postFrameHint,
			_duration, _time, &_keyFrame->positionKey);

		rotation = compute_VectorFromKeyFrame<aiQuatKey, aiQuaternion>(
			_boneKeyframe.rotationKeyFrame,
			preFrameHint, postFrameHint,
			_duration, _time, &_keyFrame->rotationKey);

		scale = compute_VectorFromKeyFrame<aiVectorKey, aiVector3D>(
			_boneKeyframe.scaleKeyFrame,
			preFrameHint, postFrameHint,
			_duration, _time, &_keyFrame->scaleKey);
	}
	else
	{
		position = compute_VectorFromKeyFrame<aiVectorKey, aiVector3D>(
			_boneKeyframe.positionKeyFrame,
			preFrameHint, postFrameHint,
			_duration, _time, nullptr);

		rotation = compute_VectorFromKeyFrame<aiQuatKey, aiQuaternion>(
			_boneKeyframe.rotationKeyFrame,
			preFrameHint, postFrameHint,
			_duration, _time, nullptr);

		scale = compute_VectorFromKeyFrame<aiVectorKey, aiVector3D>(
			_boneKeyframe.scaleKeyFrame,
			preFrameHint, postFrameHint,
			_duration, _time, nullptr);
	}

	D3DXMATRIX t, r, s;
	D3DXMATRIX tt, tr;

	D3DXMatrixTranspose(&tt, D3DXMatrixTranslation(&t, position.x, position.y, position.z));
	D3DXMatrixTranspose(&tr, D3DXMatrixRotationQuaternion(&r, &D3DXQUATERNION(rotation.x, rotation.y, rotation.z, rotation.w)));
	D3DXMatrixScaling(&s, scale.x, scale.y, scale.z);

	return tt * tr * s;
}