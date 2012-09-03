/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        core/CRenderItemManager.cpp
*  PURPOSE:
*  DEVELOPERS:  idiot
*
*****************************************************************************/

#include "StdInc.h"
#include <game/CGame.h>


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::CRenderItemManager
//
//
//
////////////////////////////////////////////////////////////////
CRenderItemManager::CRenderItemManager ( void )
{
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::~CRenderItemManager
//
//
//
////////////////////////////////////////////////////////////////
CRenderItemManager::~CRenderItemManager ( void )
{
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::OnDeviceCreate
//
//
//
////////////////////////////////////////////////////////////////
void CRenderItemManager::OnDeviceCreate ( IDirect3DDevice9* pDevice, float fViewportSizeX, float fViewportSizeY )
{
    m_pDevice = pDevice;
    m_uiDefaultViewportSizeX = fViewportSizeX;
    m_uiDefaultViewportSizeY = fViewportSizeY;

    m_pRenderWare = CCore::GetSingleton ().GetGame ()->GetRenderWare ();
    m_pRenderWare->InitWorldTextureWatch ( StaticWatchCallback );
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::OnLostDevice
//
// Release device stuff
//
////////////////////////////////////////////////////////////////
void CRenderItemManager::OnLostDevice ( void )
{
    for ( std::set < CRenderItem* >::iterator iter = m_CreatedItemList.begin () ; iter != m_CreatedItemList.end () ; iter++ )
        (*iter)->OnLostDevice ();
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::OnResetDevice
//
// Recreate device stuff
//
////////////////////////////////////////////////////////////////
void CRenderItemManager::OnResetDevice ( void )
{
    for ( std::set < CRenderItem* >::iterator iter = m_CreatedItemList.begin () ; iter != m_CreatedItemList.end () ; iter++ )
        (*iter)->OnResetDevice ();
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::CreateTexture
//
// TODO: Make underlying data for textures shared
//
////////////////////////////////////////////////////////////////
CTextureItem* CRenderItemManager::CreateTexture ( const SString& strFullFilePath )
{
    CFileTextureItem* pTextureItem = new CFileTextureItem ();
    pTextureItem->PostConstruct ( this, strFullFilePath );

    if ( !pTextureItem->IsValid () )
    {
        SAFE_RELEASE ( pTextureItem );
        return NULL;
    }

    return pTextureItem;
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::CreateRenderTarget
//
//
//
////////////////////////////////////////////////////////////////
CRenderTargetItem* CRenderItemManager::CreateRenderTarget ( uint uiSizeX, uint uiSizeY, bool bWithAlphaChannel )
{
    CRenderTargetItem* pRenderTargetItem = new CRenderTargetItem ();
    pRenderTargetItem->PostConstruct ( this, uiSizeX, uiSizeY, bWithAlphaChannel );

    if ( !pRenderTargetItem->IsValid () )
    {
        SAFE_RELEASE ( pRenderTargetItem );
        return NULL;
    }

    return pRenderTargetItem;
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::CreateScreenSource
//
//
//
////////////////////////////////////////////////////////////////
CScreenSourceItem* CRenderItemManager::CreateScreenSource ( uint uiSizeX, uint uiSizeY )
{
    CScreenSourceItem* pScreenSourceItem = new CScreenSourceItem ();
    pScreenSourceItem->PostConstruct ( this, uiSizeX, uiSizeY );

    if ( !pScreenSourceItem->IsValid () )
    {
        SAFE_RELEASE ( pScreenSourceItem );
        return NULL;
    }

    return pScreenSourceItem;
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::CreateShader
//
// Create a D3DX effect from .fx file
//
////////////////////////////////////////////////////////////////
CShaderItem* CRenderItemManager::CreateShader ( const SString& strFullFilePath, const SString& strRootPath, SString& strOutStatus, bool bDebug )
{
    strOutStatus = "";

    CShaderItem* pShaderItem = new CShaderItem ();
    pShaderItem->PostConstruct ( this, strFullFilePath, strRootPath, strOutStatus, bDebug );

    if ( !pShaderItem->IsValid () )
    {
        SAFE_RELEASE ( pShaderItem );
        return NULL;
    }

    return pShaderItem;
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::CreateDxFont
//
// TODO: Make underlying data for fonts shared
//
////////////////////////////////////////////////////////////////
CDxFontItem* CRenderItemManager::CreateDxFont ( const SString& strFullFilePath, uint uiSize, bool bBold )
{
    CDxFontItem* pDxFontItem = new CDxFontItem ();
    pDxFontItem->PostConstruct ( this, strFullFilePath, uiSize, bBold );

    if ( !pDxFontItem->IsValid () )
    {
        SAFE_RELEASE ( pDxFontItem );
        return NULL;
    }

    return pDxFontItem;
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::CreateGuiFont
//
// TODO: Make underlying data for fonts shared
//
////////////////////////////////////////////////////////////////
CGuiFontItem* CRenderItemManager::CreateGuiFont ( const SString& strFullFilePath, const SString& strFontName, uint uiSize )
{
    CGuiFontItem* pGuiFontItem = new CGuiFontItem ();
    pGuiFontItem->PostConstruct ( this, strFullFilePath, strFontName, uiSize );

    if ( !pGuiFontItem->IsValid () )
    {
        SAFE_RELEASE ( pGuiFontItem );
        return NULL;
    }

    return pGuiFontItem;
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::ReleaseRenderItem
//
// Decrement reference count on a render item, and delete if necessary
//
////////////////////////////////////////////////////////////////
void CRenderItemManager::ReleaseRenderItem ( CRenderItem* pItem )
{
    SAFE_RELEASE ( pItem );
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::NotifyContructRenderItem
//
// Add to managers list
//
////////////////////////////////////////////////////////////////
void CRenderItemManager::NotifyContructRenderItem ( CRenderItem* pItem )
{
    assert ( !MapContains ( m_CreatedItemList, pItem ) );
    MapInsert ( m_CreatedItemList, pItem );

    if ( CScreenSourceItem* pScreenSourceItem = DynamicCast < CScreenSourceItem > ( pItem ) )
        m_bBackBufferCopyMaybeNeedsResize = true;
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::NotifyDestructRenderItem
//
// Remove from managers list
//
////////////////////////////////////////////////////////////////
void CRenderItemManager::NotifyDestructRenderItem ( CRenderItem* pItem )
{
    assert ( MapContains ( m_CreatedItemList, pItem ) );
    MapRemove ( m_CreatedItemList, pItem );

    if ( CScreenSourceItem* pScreenSourceItem = DynamicCast < CScreenSourceItem > ( pItem ) )
        m_bBackBufferCopyMaybeNeedsResize = true;
    else
    if ( CShaderItem* pShaderItem = DynamicCast < CShaderItem > ( pItem ) )
        RemoveShaderItemFromWatchLists ( pShaderItem );
}



//
//
// Shaders
//
//

////////////////////////////////////////////////////////////////
//
// CRenderItemManager::SetShaderValue
//
// Set one texture
//
////////////////////////////////////////////////////////////////
bool CRenderItemManager::SetShaderValue ( CShaderItem* pShaderItem, const SString& strName, CTextureItem* pTextureItem )
{
    return pShaderItem->SetValue ( strName, pTextureItem );
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::SetShaderValue
//
// Set one bool
//
////////////////////////////////////////////////////////////////
bool CRenderItemManager::SetShaderValue ( CShaderItem* pShaderItem, const SString& strName, bool bValue )
{
    return pShaderItem->SetValue ( strName, bValue );
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::SetShaderValue
//
// Set up to 16 floats
//
////////////////////////////////////////////////////////////////
bool CRenderItemManager::SetShaderValue ( CShaderItem* pShaderItem, const SString& strName, const float* pfValues, uint uiCount )
{
    return pShaderItem->SetValue ( strName, pfValues, uiCount );
}



//
//
// Render targets and back buffers
//
//

////////////////////////////////////////////////////////////////
//
// CRenderItemManager::UpdateBackBufferCopy
//
// Save back buffer pixels in our special place
//
////////////////////////////////////////////////////////////////
void CRenderItemManager::UpdateBackBufferCopy ( void )
{
    // Do this here for now
    m_pRenderWare->PulseWorldTextureWatch ();

    // and this
    m_PrevFrameTextureUsage = m_FrameTextureUsage;
    m_FrameTextureUsage.clear ();


    //
    // UpdateBackBufferCopy
    //

    if ( m_bBackBufferCopyMaybeNeedsResize )
        UpdateBackBufferCopySize ();

    // Don't bother doing this unless at least one screen source in active
    if ( !m_pBackBufferCopy )
        return;

    // Try to get the back buffer
	IDirect3DSurface9* pD3DBackBufferSurface = NULL;
    m_pDevice->GetBackBuffer ( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pD3DBackBufferSurface );
    if ( !pD3DBackBufferSurface )
        return;

    // Copy back buffer into our private render target
    D3DTEXTUREFILTERTYPE FilterType = D3DTEXF_LINEAR;
    HRESULT hr = m_pDevice->StretchRect( pD3DBackBufferSurface, NULL, m_pBackBufferCopy->m_pD3DRenderTargetSurface, NULL, FilterType );

    m_uiBackBufferCopyRevision++;

    // Clean up
	SAFE_RELEASE( pD3DBackBufferSurface );
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::UpdateScreenSource
//
// Copy from back buffer store to screen source
// TODO - Optimize the case where the screen source is the same size as the back buffer copy (i.e. Use back buffer copy resources instead)
//
////////////////////////////////////////////////////////////////
void CRenderItemManager::UpdateScreenSource ( CScreenSourceItem* pScreenSourceItem )
{
    // Only do update if back buffer copy has changed
    if ( pScreenSourceItem->m_uiRevision == m_uiBackBufferCopyRevision )
        return;

    pScreenSourceItem->m_uiRevision = m_uiBackBufferCopyRevision;

    if ( m_pBackBufferCopy )
    {
        D3DTEXTUREFILTERTYPE FilterType = D3DTEXF_LINEAR;
        m_pDevice->StretchRect( m_pBackBufferCopy->m_pD3DRenderTargetSurface, NULL, pScreenSourceItem->m_pD3DRenderTargetSurface, NULL, FilterType );
    }
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::UpdateBackBufferCopySize
//
// Create/resize/destroy back buffer copy depending on what is required
//
////////////////////////////////////////////////////////////////
void CRenderItemManager::UpdateBackBufferCopySize ( void )
{
    m_bBackBufferCopyMaybeNeedsResize = false;

    // Set what the max size requirement is for the back buffer copy
    uint uiSizeX = 0;
    uint uiSizeY = 0;
    for ( std::set < CRenderItem* >::iterator iter = m_CreatedItemList.begin () ; iter != m_CreatedItemList.end () ; iter++ )
    {
        if ( CScreenSourceItem* pScreenSourceItem = DynamicCast < CScreenSourceItem > ( *iter ) )
        {
            uiSizeX = Max ( uiSizeX, pScreenSourceItem->m_uiSizeX );
            uiSizeY = Max ( uiSizeY, pScreenSourceItem->m_uiSizeY );
        }
    }

    // Update?
    if ( !m_pBackBufferCopy || m_pBackBufferCopy->m_uiSizeX != uiSizeX || m_pBackBufferCopy->m_uiSizeY != uiSizeY )
    {
        // Delete old one if it exists
        if ( m_pBackBufferCopy )
        {
            ReleaseRenderItem ( m_pBackBufferCopy );
            m_pBackBufferCopy = NULL;
        }

        // Try to create new one if needed
        if ( uiSizeX > 0 )
            m_pBackBufferCopy = CreateRenderTarget ( uiSizeX, uiSizeY, false );
    }
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::SetRenderTarget
//
// Set current render target to a custom one
//
////////////////////////////////////////////////////////////////
bool CRenderItemManager::SetRenderTarget ( CRenderTargetItem* pItem, bool bClear )
{
    if ( !CGraphics::GetSingleton().CanSetRenderTarget () )
        return false;

    ChangeRenderTarget ( pItem->m_uiSizeX, pItem->m_uiSizeY, pItem->m_pD3DRenderTargetSurface, pItem->m_pD3DZStencilSurface );

    if ( bClear )
        m_pDevice->Clear ( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0,0,0,0), 1, 0 );

    return true;
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::SaveDefaultRenderTarget
//
// Save current render target as the default one
//
////////////////////////////////////////////////////////////////
bool CRenderItemManager::SaveDefaultRenderTarget ( void )
{
    // Update our info about what rendertarget is active
    IDirect3DSurface9* pActiveD3DRenderTarget;
    IDirect3DSurface9* pActiveD3DZStencilSurface;
    m_pDevice->GetRenderTarget ( 0, &pActiveD3DRenderTarget );
    m_pDevice->GetDepthStencilSurface ( &pActiveD3DZStencilSurface );

    m_pDefaultD3DRenderTarget = pActiveD3DRenderTarget;
    m_pDefaultD3DZStencilSurface = pActiveD3DZStencilSurface;

    // Don't hold any refs because it goes all funny during fullscreen minimize/maximize, even if they are released at onLostDevice
    SAFE_RELEASE ( pActiveD3DRenderTarget )
    SAFE_RELEASE ( pActiveD3DZStencilSurface )

    return true;
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::RestoreDefaultRenderTarget
//
// Set render target back to the default one
//
////////////////////////////////////////////////////////////////
bool CRenderItemManager::RestoreDefaultRenderTarget ( void )
{
    if ( !CGraphics::GetSingleton().CanSetRenderTarget () )
        return false;

    // Only need to change if we have info
    if ( m_pDefaultD3DRenderTarget )
        ChangeRenderTarget ( m_uiDefaultViewportSizeX, m_uiDefaultViewportSizeY, m_pDefaultD3DRenderTarget, m_pDefaultD3DZStencilSurface );

    return true;
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::ChangeRenderTarget
//
// Worker function to change the D3D render target
//
////////////////////////////////////////////////////////////////
void CRenderItemManager::ChangeRenderTarget ( uint uiSizeX, uint uiSizeY, IDirect3DSurface9* pD3DRenderTarget, IDirect3DSurface9* pD3DZStencilSurface )
{
    // Check if we need to change
    IDirect3DSurface9* pCurrentRenderTarget;
    IDirect3DSurface9* pCurrentZStencilSurface;
    m_pDevice->GetRenderTarget ( 0, &pCurrentRenderTarget );
    m_pDevice->GetDepthStencilSurface ( &pCurrentZStencilSurface );

    bool bAlreadySet = ( pD3DRenderTarget == pCurrentRenderTarget && pD3DZStencilSurface == pCurrentZStencilSurface );

    SAFE_RELEASE ( pCurrentRenderTarget );
    SAFE_RELEASE ( pCurrentZStencilSurface );

    // Already set?
    if ( bAlreadySet )
        return;

    // Tell graphics things are about to change
    CGraphics::GetSingleton().OnChangingRenderTarget ( uiSizeX, uiSizeY );

    // Do change
    m_pDevice->SetRenderTarget ( 0, pD3DRenderTarget );
    m_pDevice->SetDepthStencilSurface ( pD3DZStencilSurface );

    D3DVIEWPORT9 viewport;
    viewport.X = 0;
    viewport.Y = 0;
    viewport.Width = uiSizeX;
    viewport.Height = uiSizeY;
    viewport.MinZ = 0.0f;
    viewport.MaxZ = 1.0f;
    m_pDevice->SetViewport ( &viewport );
}



//
//
// Apply shaders to GTA objects
//
//

////////////////////////////////////////////////////////////////
//
// CRenderItemManager::GetAppliedShaderForD3DData
//
// Find which shader item is being used to render this D3DData
//
////////////////////////////////////////////////////////////////
CShaderItem* CRenderItemManager::GetAppliedShaderForD3DData ( CD3DDUMMY* pD3DData )
{
    // Save texture usage for later
    MapInsert ( m_FrameTextureUsage, pD3DData );

    // Find matching shader
    CShaderItem** ppShaderItem = (CShaderItem**)MapFind ( m_D3DDataShaderMap, pD3DData );
    if ( ppShaderItem )
        return *ppShaderItem;
    return NULL;
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::GetVisibleTextureNames
//
// Get the names of all streamed in world textures, filtered by name and/or model
//
////////////////////////////////////////////////////////////////
void CRenderItemManager::GetVisibleTextureNames ( std::vector < SString >& outNameList, const SString& strTextureNameMatch, ushort usModelID )
{
    // If modelid supplied, get the model texture names into a map
    std::set < SString > modelTextureNameMap;
    if ( usModelID )
    {
        std::vector < SString > modelTextureNameList;
        m_pRenderWare->GetModelTextureNames ( modelTextureNameList, usModelID );
        for ( std::vector < SString >::const_iterator iter = modelTextureNameList.begin () ; iter != modelTextureNameList.end () ; ++iter )
            modelTextureNameMap.insert ( (*iter).ToLower () );
    }

    SString strTextureNameMatchLower = strTextureNameMatch.ToLower ();

    std::set < SString > resultMap;

    // For each texture that was used in the previous frame
    for ( std::set < CD3DDUMMY* >::const_iterator iter = m_PrevFrameTextureUsage.begin () ; iter != m_PrevFrameTextureUsage.end () ; ++iter )
    {
        // Get the texture name
        SString strTextureName = m_pRenderWare->GetTextureName ( *iter );
        SString strTextureNameLower = strTextureName.ToLower ();

        if ( strTextureName.empty () )
            continue;

        // Filter by wildcard match
        if ( !WildcardMatch ( strTextureNameMatchLower, strTextureNameLower ) )
            continue;

        // Filter by model
        if ( usModelID )
            if ( !MapContains ( modelTextureNameMap, strTextureNameLower ) )
                continue;

        resultMap.insert ( strTextureName );
    }

    for ( std::set < SString >::const_iterator iter = resultMap.begin () ; iter != resultMap.end () ; ++iter )
    {
        outNameList.push_back ( *iter );
    }
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::ApplyShaderItemToWorldTexture
//
// Add an association between the shader item and a world texture match
//
////////////////////////////////////////////////////////////////
bool CRenderItemManager::ApplyShaderItemToWorldTexture ( CShaderItem* pShaderItem, const SString& strTextureNameMatch, float fOrderPriority )
{
    assert ( pShaderItem );

    // Remove any existing match
    RemoveShaderItemFromWorldTexture ( pShaderItem, strTextureNameMatch );

    // Add new match at the end
    return m_pRenderWare->AddWorldTextureWatch ( (CSHADERDUMMY*)pShaderItem, strTextureNameMatch, fOrderPriority );
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::RemoveShaderItemFromWorldTexture
//
// Remove an association between the shader item and the world texture
//
////////////////////////////////////////////////////////////////
bool CRenderItemManager::RemoveShaderItemFromWorldTexture ( CShaderItem* pShaderItem, const SString& strTextureNameMatch )
{
    assert ( pShaderItem );

    m_pRenderWare->RemoveWorldTextureWatch ( (CSHADERDUMMY*)pShaderItem, strTextureNameMatch );
    return true;
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::RemoveShaderItemFromWatchLists
//
// Ensure the shader item is not being referred to by the texture replacement system
//
////////////////////////////////////////////////////////////////
void CRenderItemManager::RemoveShaderItemFromWatchLists ( CShaderItem* pShaderItem )
{
    //
    // Remove shader item from all world textures
    //
    m_pRenderWare->RemoveWorldTextureWatchByContext ( (CSHADERDUMMY*)pShaderItem );

    //
    // Remove shader item from D3DData map
    //
    for ( std::map < CD3DDUMMY*, CSHADERDUMMY* >::iterator dataIter = m_D3DDataShaderMap.begin () ; dataIter != m_D3DDataShaderMap.end () ; )
    {
        if ( pShaderItem == (CShaderItem*)dataIter->second )
            m_D3DDataShaderMap.erase ( dataIter++ );
        else
            ++dataIter;
    }
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::WatchCallback
//
// Notification that the pD3DData is being changed for this context
//
////////////////////////////////////////////////////////////////
void CRenderItemManager::WatchCallback ( CSHADERDUMMY* pShaderItem, CD3DDUMMY* pD3DDataNew, CD3DDUMMY* pD3DDataOld )
{
    // Remove old pointer
    if ( pD3DDataOld )
        MapRemove ( m_D3DDataShaderMap, pD3DDataOld );

    // Add new pointer
    if ( pD3DDataNew )
        MapSet ( m_D3DDataShaderMap, pD3DDataNew, pShaderItem );
}


////////////////////////////////////////////////////////////////
//
// CRenderItemManager::StaticWatchCallback
//
//
//
////////////////////////////////////////////////////////////////
void CRenderItemManager::StaticWatchCallback ( CSHADERDUMMY* pShaderItem, CD3DDUMMY* pD3DDataNew, CD3DDUMMY* pD3DDataOld )
{
    ( ( CRenderItemManager* ) CGraphics::GetSingleton ().GetRenderItemManager () )->WatchCallback ( pShaderItem, pD3DDataNew, pD3DDataOld );
}