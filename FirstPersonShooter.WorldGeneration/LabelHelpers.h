#pragma once
namespace WorldGenerator
{

	class __declspec(dllexport) LabelHelpers
	{
	private:
		static const int WILD_CARD_LABEL = -1;
	public:
		static bool MatchLabels(int l1, int l2);
		static int TransformLabel(int old_label, int new_label);
	};
}

