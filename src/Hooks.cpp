#include "Hooks.h"
#include "Manager.h"

namespace BaseObjectSwapper
{
	void detail::swap_base(RE::TESObjectREFR* a_ref)
	{
		if (const auto base = a_ref->GetObjectReference(); base) {
			FormSwap::Manager::GetSingleton()->LoadFormsOnce();

            RE::BGSMaterialSwap* swapForm{ nullptr };
			if (const auto materialSwap = base->As<RE::BGSModelMaterialSwap>()) {
				swapForm = materialSwap->swapForm;
			}

		    const auto& [swapBase, transformData] = FormSwap::Manager::GetSingleton()->GetSwapData(a_ref, base, swapForm);
			if (swapBase && swapBase != base) {
				a_ref->SetObjectReference(swapBase);
			}
			if (transformData) {
				transformData->SetTransform(a_ref);
			}
		}
	}

	void Install()
	{
		logger::info("{:*^30}", "HOOKS");

		InitItemImpl<RE::TESObjectREFR>::Install();
		InitItemImpl<RE::Hazard>::Install();
		InitItemImpl<RE::ArrowProjectile>::Install();
	}
}
