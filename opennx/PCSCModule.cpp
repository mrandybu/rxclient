#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/filesys.h>
#include <wx/config.h>

#ifdef APP_OPENNX
# include "opennxApp.h"
#endif

#include "trace.h"
ENABLE_TRACE;

#include "MyXmlConfig.h"
#include "PCSCModule.h"

class wxConfigBase;
// ----------------------------------------------------------------------------
std::shared_ptr<IModule> PCSCModule::create()
{
    auto pcsc = std::make_shared<PCSCModule>();
    return std::static_pointer_cast<IModule>(pcsc);
}
// ----------------------------------------------------------------------------
PCSCModule::PCSCModule()
    :IModule(wxT("pcsc"))
{
    wxString sysDir;
    wxConfigBase::Get()->Read(wxT("Config/SystemNxDir"), &sysDir);

    smartcard = std::make_shared<wxFileName>(sysDir, wxEmptyString);
    pcsc = std::make_shared<wxFileName>(sysDir, wxEmptyString);

    smartcard->AppendDir(wxT("bin"));
    smartcard->SetName(wxT("nxssh.ext.sh"));

    pcsc->AppendDir(wxT("bin"));
    pcsc->SetName(wxT("nx-pcsc-helper.sh"));
}

// ----------------------------------------------------------------------------
bool PCSCModule::exist() {
    return smartcard->FileExists() || pcsc->FileExists();
}
// ----------------------------------------------------------------------------
wxString PCSCModule::getNxSshCmd( const wxString& defaultName ) const
{
    if( smartcard->FileExists() )
        return wxT("nxssh.ext.sh");

    return IModule::getNxSshCmd(defaultName);
}
// ----------------------------------------------------------------------------
wxString PCSCModule::getNxSshExtraParam( const MyXmlConfig* pCfg ) const
{
    if( smartcard->FileExists() )
    {
        wxString p;
        p << wxT(" pcsc ") << pCfg->sGetUsername();
        return p;
    }

    return IModule::getNxSshExtraParam(pCfg);
}
// ----------------------------------------------------------------------------
wxString PCSCModule::getSessionExtraParam( const MyXmlConfig *pCfg ) const
{
    //
    // format: ' --param1=val1 --param2=val2  ...'
    //

    if( smartcard->FileExists() || pcsc->FileExists() )
    {
        // WARNING:
        // 'smartcard' uses --pcscd=[0,1]
        // 'pcsc' uses --pcscd='portForForwarding'
        // --pcscd is using nxssh for forwarding port
        // see details in (RU) https://bugs.etersoft.ru/show_bug.cgi?id=12712
        wxString p;

        // 'sshsharing' used only for 'smartcard' module
        if( smartcard->FileExists() )
            p << wxT(" sshsharing");

        p << wxT(" --pcscd=\"") << pCfg->iGetPcscPort() << wxT("\"");
        return p;
    }

    return IModule::getSessionExtraParam(pCfg);
}
// ----------------------------------------------------------------------------
wxString PCSCModule::getNxProxyExtraParam( const MyXmlConfig* pCfg ) const
{
    //
    // format: ,param1=val1,param2=val2,...
    //

    if( pcsc->FileExists() )
    {
        wxString p;
        p << wxT(",pcscd=") << pCfg->iGetPcscPort();
        return p;
    }

    return IModule::getNxProxyExtraParam(pCfg);
}
// ----------------------------------------------------------------------------
void PCSCModule::runAfterNxSsh( const MyXmlConfig* pCfg, int nxsshPID )
{
    if( pcsc->FileExists() )
    {
        wxString cmd = pcsc->GetShortPath();
        cmd << wxT(" client ") << nxsshPID
            << wxT(" ") << pCfg->sGetPcscSocketPath()
            << wxT(" ") << pCfg->iGetPcscPort();

        ::myLogTrace(MYTRACETAG, wxT("(pcsc): executing %s"), to_c_str(cmd));
        wxExecute(cmd);
    }
}
// ----------------------------------------------------------------------------

