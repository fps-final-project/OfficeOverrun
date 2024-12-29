#include "pch.h"
#include "LabelHelpers.h"

using namespace WorldGenerator;

bool LabelHelpers::MatchLabels(int l1, int l2)
{
	if (l1 == WILD_CARD_LABEL || l2 == WILD_CARD_LABEL)
		return true;
	else
		return l1 == l2;
}

int WorldGenerator::LabelHelpers::TransformLabel(int old_label, int new_label)
{
	if (new_label == WILD_CARD_LABEL)
		return old_label;
	else
		return new_label;
}
