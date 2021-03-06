#include "j1App.h"
#include "Render.h"
#include "Fonts.h"
#include "UILabel.h"
#include "p2Log.h"

UILabel::UILabel(iPoint pos, _TTF_Font* font, std::string text, SDL_Color color, int max_width, bool image)
{
	int w, h;
	App->fonts->CalcSize("a", w, h, font); //calc size of one char

	if (max_width > 0)
	{
		//calculate the amount of lines that string has relative to the max width
		int lines = 1;
		int char_in_line = 0;
		int max_chars_in_line = max_width / w;
		for (int i = 0; i < text.size(); i++)
		{
			if (text.c_str()[i] == '\n')
			{
				lines++;
				char_in_line = 0;
			}
			else if (char_in_line > max_chars_in_line)
			{
				lines++;
				char_in_line = 0;
			}
			char_in_line++;
		}

		rect_box.w = max_width;
		rect_box.h = h * lines;
	}
	else
	{
		//calc amount of lines independent of a max width
		int lines = 1;
		int max_chars_in_line = 0;
		int chars_in_line = 0;
		for (int i = 0; i < text.size(); i++)
		{
			if (text.c_str()[i] == '\n')
			{
				lines++;
				max_chars_in_line = chars_in_line > max_chars_in_line ? chars_in_line : max_chars_in_line;
				chars_in_line = 0;
			}
			chars_in_line++;
		}
		max_chars_in_line = chars_in_line > max_chars_in_line ? chars_in_line : max_chars_in_line;

		//adjust size of box to the chars and lines
		rect_box.w = w * max_chars_in_line;
		rect_box.h = h * lines;
	}

	rect_box.x = pos.x;
	rect_box.y = pos.y;

	this->font = font;
	this->text = text;
	this->color = color;
	this->image = image;
}
UILabel::~UILabel()
{
}

bool UILabel::UIBlit()
{
	iPoint screen_pos = GetScreenPos();
	SDL_Texture* texture = App->fonts->Print(text.c_str(), color, font);
	if (image) 
	{
		if (clipping && parent)
			App->render->Blit(texture, screen_pos.x, screen_pos.y, nullptr, 0.0F, 0.0, INT_MAX, INT_MAX);
		else
			App->render->Blit(texture, screen_pos.x, screen_pos.y, nullptr, 0.0F, 0.0, INT_MAX, INT_MAX);
		LOG("Blitea");
	}


	SDL_DestroyTexture(texture);
	return true;
}

void UILabel::SetText(std::string text)
{
	this->text = text;
}

void UILabel::SetColor(SDL_Color color)
{
	this->color = color;
}

bool UILabel::CleanUp()
{
	parent = nullptr;
	font = nullptr;

	return true;
}
