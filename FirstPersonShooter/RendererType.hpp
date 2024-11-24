#pragma once

// so this is kind of a code smell but we do not expect to have too many renderer types so let's live with that for now
enum class __declspec(dllexport) RendererType { ANIMATED, MODEL };
