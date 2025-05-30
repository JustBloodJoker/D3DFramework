#include "../pch.h"
#include "RootSignature.h"

namespace FD3DW
{




	RootSingature::RootSingature(UINT numParameters) 
	{
		m_vSlotRootParameters.reserve(numParameters);
	}

	RootSingature::RootSingature(CD3DX12_ROOT_PARAMETER* m_vSlotRootParameters, UINT numParameters) : m_vSlotRootParameters(m_vSlotRootParameters, m_vSlotRootParameters + numParameters)
	{
	}

	void RootSingature::CreateRootSignature(ID3D12Device* pDevice, const D3D12_ROOT_SIGNATURE_FLAGS flags, const UINT samplersCount, D3D12_STATIC_SAMPLER_DESC* samplers, const UINT node)
	{
		wrl::ComPtr<ID3DBlob> serializedRootSig;
		wrl::ComPtr<ID3DBlob> errorBlob;

		CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(static_cast<UINT>(m_vSlotRootParameters.size()),
			m_vSlotRootParameters.data(), samplersCount, samplers, flags);

		hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, serializedRootSig.ReleaseAndGetAddressOf(), errorBlob.ReleaseAndGetAddressOf());
		
		if (FAILED(hr)) 
		{
			if (errorBlob) 
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			}
			HRESULT_ASSERT(hr, "D3D12 Serialize Root Signature Error");
		}

		HRESULT_ASSERT(pDevice->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(m_pRootSignature.ReleaseAndGetAddressOf())), "Root signature create error");
		CONSOLE_MESSAGE("Root Signature created");
	}

	void RootSingature::AddRootParameter(const CD3DX12_ROOT_PARAMETER& rootParameter)
	{
		m_vSlotRootParameters.push_back(rootParameter);
	}

	ID3D12RootSignature* RootSingature::GetRootSignature() const
	{
		SAFE_ASSERT(m_pRootSignature.Get(), "Root Signature not created. Use CreateRootSignature after set up Root Signature data");

		return m_pRootSignature.Get();
	}

}
