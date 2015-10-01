#include <game/gameobject.h>
#include <game/actionbase.h>
#include <game/controller.h>
#include <game/modelrenderer.h>
#include <game/datatable/buildingdatatable.h>



using namespace hsdk;
using namespace game;


// 메인 루프
int main(void)
{
	GameObject obj(
		new datatable::BuildingDataTable(
		L"Default", 0, 0, 100, 0),
		new Controller(),
		new ActionBase(),
		new ModelRenderer());

	obj.controller()->attack(0);
	obj.controller()->disappear(0);
	obj.controller()->effect(0, 0);
	obj.controller()->move(0.0f, 0.0f, 0);
	obj.controller()->recovery(0, 0, 0);
	obj.controller()->resurrect(0);
	obj.controller()->suffer(0, 0, 0);
	obj.controller()->wait(0);	
}