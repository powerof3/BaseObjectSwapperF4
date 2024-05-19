#include "Hooks.h"

namespace BaseObjectSwapper
{
	void detail::swap_base(RE::TESObjectREFR* a_ref)
	{
		if (const auto base = a_ref->GetObjectReference()) {
			FormSwap::Manager::GetSingleton()->LoadFormsOnce();

			const RE::BGSMaterialSwap* materialSwapForm{ nullptr };
			if (const auto materialSwap = base->As<RE::BGSModelMaterialSwap>()) {
				materialSwapForm = materialSwap->swapForm;
			}
			
			const auto& [swapBase, objectProperties] = FormSwap::Manager::GetSingleton()->GetSwapData(a_ref, base, materialSwapForm);

			if (swapBase && swapBase != base) {
				a_ref->SetObjectReference(swapBase);

				if (a_ref->extraList && a_ref->extraList->HasType(RE::EXTRA_DATA_TYPE::kLevelItem)) {
					FormSwap::Manager::GetSingleton()->InsertLeveledItemRef(a_ref);
				}
			}

			if (objectProperties) {
				objectProperties->SetTransform(a_ref);
				objectProperties->SetRecordFlags(a_ref);
			}
		}
	}

	void Install()
	{
		logger::info("{:*^30}", "HOOKS");

		InitItemImpl<RE::TESObjectREFR>::Install();
		InitItemImpl<RE::Hazard>::Install();
		InitItemImpl<RE::ArrowProjectile>::Install();

		SetObjectReference<RE::TESObjectREFR>::Install();
		SetObjectReference<RE::Hazard>::Install();
		SetObjectReference<RE::ArrowProjectile>::Install();
	}
}
