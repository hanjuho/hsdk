#include <hsdk/win/direct3d/d3d10_animation.h>


//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
DECL_FUNC_T(void, updatePos)(
	_Out_ D3DXMATRIX * _matrixBuffer,
	_In_ const hsdk::direct3d::D3D10MY_BONE * _boneBuffer,
	_In_ const bool * _animateBoneBuffer,
	_In_ unsigned int _countBuffer)
{
	for (unsigned int index = 0; index < _countBuffer; ++index)
	{
		if (_animateBoneBuffer[index])
		{
			_matrixBuffer[index] = _matrixBuffer[_boneBuffer[index].parent] * _matrixBuffer[index];
		}
		else
		{
			const hsdk::direct3d::D3D10MY_BONE & bone = _boneBuffer[index];
			_matrixBuffer[index] = _matrixBuffer[bone.parent] * bone.mRelation;
		}
	}
}

//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------

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
IMPL_FUNC(hsdk::direct3d::animation::create_Pos)(
	_Out_ D3D10_Animation_Recorder & _pos,
	_In_ const D3D10_Animation & _animation,
	_In_ unsigned int _animationPos,
	_In_ double _time)
{
	unsigned int limit = _animation.bones.size();
	if (limit == 0 || _animationPos < _animation.animations.size())
	{
		return E_FAIL;
	}

	_pos.pos.resize(limit);
	_pos.keyframe.resize(_animation.animations[_animationPos].boneKeyFrames.size);
	memset(&_pos.keyframe[0], -1, sizeof(D3D10MY_KEYFRAME_RECORD)* _pos.keyframe.size());

	D3DXMATRIX * dispatch = &_pos.pos[0];
	D3DXMatrixIdentity(&dispatch[0]);

	bool animateBone[256] = { 0 };
	{
		const D3D10MY_ANIMATION & animation = 
		const unsigned int length = animation.boneKeyFrames.size();

		{
		}

		double duration = animation.duration;
		double time = _time * animation.tickPerSecond;
		if (time < duration)
		{
			_pos.time = _time;
		}
		else
		{
			time = std::fmod(time, duration);
			_pos.time = time / duration;
		}

		for (unsigned int index = 0; index < length; ++index)
		{
			const D3D10MY_BONE_KEYFRAME & boneKeyFrame = animation.boneKeyFrames[index];

			dispatch[boneKeyFrame.boneID] = compute_MatrixFromBoneKeyFrame(
				boneKeyFrame, duration, time, &_pos.keyframe[index]);

			animateBone[boneKeyFrame.boneID] = true;
		}

	}

	updatePos(
		dispatch,
		&_animation.bones[0],
		animateBone,
		limit);

	_pos.aniamtionID = _animationPos;

	return S_OK;
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, hsdk::direct3d::animation::animate_Pos)(
	_Out_ D3D10_Animation_Recorder & _pos,
	_In_ const D3D10_Animation & _animation)
{
	D3DXMATRIX * dispatch = &_pos.pos[0];
	bool animateBone[256] = { 0 };
	{
		const D3D10MY_ANIMATION & animation = 
			_animation.animations[_pos.aniamtionID];

		double duration = animation.duration;
		double time = _pos.time * animation.tickPerSecond;
		
		// 시간 초과
		if (duration < time)
		{
			// 최대 시간 만큼 감소
			time -= duration;

			// 그럼에도 크다면
			if (duration < time)
			{
				// 최대 시간으로 나머지를 구함(오버헤드가 높음)
				time = std::fmod(time, duration);
			}

			// 새로운 타임을 저장
			_pos.time = time / animation.tickPerSecond;
		}
		// 시간이 0보다 작음
		else if (time < 0)
		{
			// 최대 시간 만큼 증가
			time += duration;

			// 그럼에도 작다면
			if (time < 0)
			{
				// 최대 시간으로 나머지를 구한 뒤 최대시간에서 감소
				time = duration - std::fmod(time, duration);
			}

			// 새로운 타임을 저장
			_pos.time = time / animation.tickPerSecond;
		}

		const unsigned int length = animation.boneKeyFrames.size();
		for (unsigned int index = 0; index < length; ++index)
		{
			const D3D10MY_BONE_KEYFRAME & boneKeyFrame = animation.boneKeyFrames[index];

			dispatch[boneKeyFrame.boneID] = compute_MatrixFromBoneKeyFrame(
			boneKeyFrame, duration, time, &_pos.keyframe[index]);

			animateBone[boneKeyFrame.boneID] = true;
		}
	}

	updatePos(
		dispatch,
		&_animation.bones[0],
		animateBone,
		_animation.bones.size());
}

//--------------------------------------------------------------------------------------
IMPL_FUNC(hsdk::direct3d::animation::trans_Pos)(
	_Out_ D3D10_Animation_Recorder & _pos,
	_In_ const D3D10_Animation & _animation,
	_In_ unsigned int _animationPos)
{
	IF_FALSE(_animationPos < _animation.animations.size())
	{
		return E_FAIL;
	}

	_pos.aniamtionID = _animationPos;
	reset_Pos(_pos, _animation);	
}

//--------------------------------------------------------------------------------------
IMPL_FUNC_T(void, hsdk::direct3d::animation::reset_Pos)(
	_Out_ D3D10_Animation_Recorder & _pos,
	_In_ const D3D10_Animation & _animation)
{
	_pos.time = 0.0f;		
	auto begin = _pos.keyframe.begin();
	auto end = _pos.keyframe.end();
	while (begin != end)
	{
		begin->positionKey = 0;
		begin->rotationKey = 0;
		begin->scaleKey = 0;

		++begin;
	}

	animate_Pos(_pos, _animation);
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