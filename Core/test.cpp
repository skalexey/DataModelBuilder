#include <iostream>
#include "CoreConfig.h"
#include "DMBCore.h"

void ModelCreationTest()
{
	std::cout << "Model creation test\n";
	dmb::Model model;
	auto& registry = model.GetRegistry();
	auto& privateScope = model.GetPrivateScope();
	auto& pear = registry.CreateType("pear");
	pear.Set("isFruit", true);
	pear.Set("color", "Yellow");
	pear.Set("radius", 0.3f);
	pear.Set("branchCount", 1);
	auto bush = vl::Object();
	bush.Set("leafColor", "Green");
	bush.Set("isTree", true);
	bush.Set("leafPerBranch", 6);
	bush.Set("branches", vl::List());
	bush.Set("x", 0);
	bush.Set("y", 0);
	registry.RegisterType("bush", bush);
	auto branch = vl::Object();
	privateScope.RegisterType("branch", branch);
	branch.Set("leafCount", 10);
	branch.Set("fruit", vl::Object());
	branch.Set("branches", vl::List());
	auto branch1 = branch.Copy();
	branch1.Set("leafCount", 9);
	auto branch2 = branch.Copy();
	branch2.Set("leafCount", 3);
	branch2.Set("fruit", pear);
	branch1.Get("branches").AsList().Add(branch2);
	bush.Get("branches").AsList().Add(branch1);
	std::cout << "Model created\n";
	const char* fName = "new_model.json";
	model.Store(fName, { true, true });
	std::cout << "Model written to the json file'" << fName << "':\n";
	std::cout << model.JSONStr({ true }) << "\n";
}

void ModelEditTest()
{
	std::cout << "Model edit test\n";
	dmb::Model model;
	const char* fName = "content_filling_test_out.json";
	model.Load(fName);
	std::cout << "Loaded from '" << fName << "': \n" << model.JSONStr({ true }) << "\n";
}

void ContentFillingTest()
{
	std::cout << "Content filling test\n";
	dmb::Model model;
	model.Load("new_model.json");
	auto& content = model.GetContent();
	auto& tree = content.Add("fruitTree", model.GetType("bush"));
	std::cout << "Object 'bush' instantiated into the content with name 'fruitTree'\n";
	tree.Set("leafColor", "Gold");
	std::cout << "leafPerBranch: " << tree.Get("leafPerBranch").AsNumber().Val() << "\n";
	content.Add("fieldSize", vl::NumberVar(4));
	auto& animals = content.Add("animalsAround", vl::List()).AsList();
	auto& registry = model.GetRegistry();
	auto& privateScope = model.GetPrivateScope();
	auto& animal = registry.CreateType("animal");
	animal.Set("type", "");
	vl::Object bear;
	bear.SetPrototype(animal);
	bear.Set("type", "bear");
	bear.Set("isMammal", true);
	vl::Object bird;
	bird.SetPrototype(animal);
	bird.Set("type", "bird");
	animals.Add(bear);
	animals.Add(bird);
	animals.Add(vl::Object());
	const char* fName = "content_filling_test_out.json";
	model.Store(fName, { true });
	std::cout << "Written to '" << fName << "': \n" << model.JSONStr({ true }) << "\n";
}

void SeparateContentStoreTest()
{
	std::cout << "Separate content store test\n";
	const char* fName = "content_filling_test_out.json";
	dmb::Model model;
	if (model.Load(fName))
		std::cout << "Model loaded from '" << fName << "'\n";
	else
	{
		std::cout << "Error while loading a model from '" << fName << "'\n";
		return;
	}
	const char* outFName = "separate_content.json";
	if (model.GetContent().Store(outFName, { true }))
		std::cout << "Content stored into the file '" << outFName << "':\n"
			<< model.GetContent().JSONStr({ true }) << "\n";
	else
		std::cout << "Error while storing the content\n";
}

void RenameTest()
{
	std::cout << "Rename test\n";
	dmb::Model model;
	const char* fName = "new_model.json";
	model.Load(fName);
	auto& registry = model.GetRegistry();
	if (registry.HasType("pear"))
	{
		if (registry.GetType("pear").RenameProperty("radius", "diameter"))
			std::cout << "Pear's 'radius' property renamed to 'diameter'\n";
		else
			std::cout << "Can't rename property 'radius'\n";
	}
	else
	{
		std::cout << "Can't find 'pear' type in the model loaded from '" << fName << "'\n";
	}
}

int main(int argc, const char* argv[])
{
	std::cout << "DataModelBuilderCore Test Project\n";
	ModelCreationTest();
	ModelEditTest();
	ContentFillingTest();
	SeparateContentStoreTest();
	RenameTest();
	return 0;
}
