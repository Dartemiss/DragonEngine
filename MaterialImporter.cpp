#include "MaterialImporter.h"

#include "Application.h"
#include "ModuleFilesystem.h"
#include "ModuleTexture.h"
#include <DevIL/il.h>
#include <DevIL/ilu.h>
#include <DevIL/ilut.h>

using namespace std;

MaterialImporter::MaterialImporter()
{
	ilInit();
	iluInit();
	ilutInit();
}

//TODO: when calling the function "Import", make sure to check first the fbx path, then model folder and last Textures folder.
bool MaterialImporter::Import(const char * path, const char * file, string & output_file)
{
	//Loading image
	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);

	string filepath = path; filepath += file;

	string file_no_ext = file;
	size_t lastindex = file_no_ext.find_last_of(".");
	file_no_ext = file_no_ext.substr(0, lastindex);

	LOG("Importing image from %s.", filepath.c_str());

	bool isLoaded = ilLoadImage(filepath.c_str());

	if (!isLoaded)
	{
		LOG("Can't load image from %s.", filepath.c_str());
		return false;
	}

	//Make sure image is in RGB or devil will return an empty string
	bool converted = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	if (!converted)
	{
		ILenum error = ilGetError();
		LOG("Error converting image to rgb: %s - %s", std::to_string(error), iluErrorString(error));
		iluDeleteImage(image);
		return false;
	}

	//Make sure image is not flipped
	ILinfo ImageInfo;
	iluGetImageInfo(&ImageInfo);
	if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
	{
		LOG("Flipping Image Origin.");
		iluFlipImage();
	}

	//Transform image into DDS
	ILuint size;
	ILubyte *data;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
	size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
	if (size > 0) {
		data = new ILubyte[size]; // allocate data buffer
		if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
			if (Import(file_no_ext.c_str(), data, size, output_file))
				LOG("Image successfully imported.");
		delete[] data;
	}
	else
		LOG("Coudn't import image from %s into own format.", filepath.c_str());

	//Delete image data
	iluDeleteImage(image);

	return false;
}

bool MaterialImporter::Import(const char* file, const void * buffer, unsigned int size, string & output_file)
{
	if (!App->filesystem->IsDirectory("../Library"))
		App->filesystem->MakeDirectory("../Library");
	if(!App->filesystem->IsDirectory("../Library/Materials"))
		App->filesystem->MakeDirectory("../Library/Materials");
	
	string filename = file; filename += ".dds";
	output_file = file;

	return App->filesystem->Save("../Library/Materials/", filename.c_str(), buffer, size, false);
}

bool MaterialImporter::Load(const char * exported_file, Texture & resource)
{
	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);

	string filepath = "../Library/Materials/"; filepath += exported_file; filepath += ".dds";
	bool isLoaded = ilLoadImage(filepath.c_str());

	if (!isLoaded)
	{
		LOG("Error loading material %s from library", exported_file);
		return false;
	}

	ILubyte* data = ilGetData();

	resource.width = ilGetInteger(IL_IMAGE_WIDTH);
	resource.height = ilGetInteger(IL_IMAGE_HEIGHT);
	resource.depth = ilGetInteger(IL_IMAGE_DEPTH);
	resource.format = ilDetermineType(exported_file);
	resource.data = data;
	resource.path = exported_file;

	resource.type = exported_file;
	size_t lastindex = resource.type.find_last_of("_");
	resource.type = resource.type.substr(lastindex);

	return true;
}
