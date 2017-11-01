enum FaxTyp:uchar {capi=1,hyla};
enum FxStat:uchar {init/*0*/,gestrichen,schwebend,wartend/*3*/,blockiert/*4*/,bereit/*5*/,verarb/*6*/,gesandt/*7*/,gescheitert/*8*/,fehlend,woasined};
enum hyinst {keineh,hysrc,hypak,hyppk}; // hyla source, hyla Paket, hylaplus Paket

class zielmustercl; // fuer die Verteilung der erfolgreich gefaxten Dateien auf verschiedene Dateien
class fxfcl; // Faxfile
class fsfcl; // Faxsendfile
class paramcl; // Programmparameter
void useruucp(const string& huser, const int obverb, const int oblog);
string zielname(const string& qdatei, const string& zielvz,uchar wieweiterzaehl=0, string* zieldatei=0, uchar* obgleichp=0, 
                int obverb=0, int oblog=0, stringstream *ausgp=0);
string zielname(const string& qdatei, const zielmustercl& zmp,uchar wieweiterzaehl=0, string* zieldatei=0, uchar* obgleichp=0, int obverb=0, 
                int oblog=0, stringstream *ausgp=0);
void dorename(const string& quelle, const string& ziel, const string& cuser=nix, uint *vfehlerp=0, uchar schonda=0, int obverb=0, int oblog=0,
                  stringstream *ausgp=0);
string verschiebe(const string& qdatei, const auto/*string,zielmustercl*/& zielvz, const string& cuser=nix, 
                  uint *vfehlerp=0, const uchar wieweiterzaehl=1, int obverb=0,int oblog=0, stringstream *ausgp=0,const uchar auchgleiche=0);
string kopiere(const string& qdatei, const string& zield, uint *kfehler, const uchar wieweiterzaehl, int obverb=0, int oblog=0);
string kopiere(const string& qdatei, const zielmustercl& zmp, uint *kfehler, const uchar wieweiterzaehl, int obverb=0, int oblog=0);
void prueffuncgettel3(DB *const Myp, const string& usr, const string& host, int obverb, int oblog);
void pruefstdfaxnr(DB *Myp, const string& usr, const string& host, const int obverb, const int oblog);
int pruefcapi(paramcl *pmp, int obverb, int oblog);
pid_t PIDausName(const char* PName, uchar klgr, uchar exakt, int obverb, int oblog);
void hfaxsetup(paramcl *pmp,int obverb=0, int oblog=0);
int hconfig(const paramcl *const pmp,const int obverb=0, const int oblog=0);
const string& pruefspool(DB *My,const string& spooltab, const string& altspool, const int obverb, const int oblog, uchar direkt=0);
void pruefouttab(DB *My, const string& touta, const int obverb, const int oblog, const uchar direkt=0);
void pruefudoc(DB *My, const string& tudoc, const int obverb, const int oblog, const uchar direkt=0);
void pruefinctab(DB *My, const string& tinca, const int obverb, const int oblog, const uchar direkt=0);
void pruefrules(int obverb, int oblog);
void pruefblack(int obverb, int oblog);
void pruefmodcron(int obverb, int oblog);

// Steuerung der Abspeicherung gesendeter Faxe je nach Muster
class zielmustercl 
{
  // beim letzten Element muss ziel leer sein!
  private:
    string muster;
  public:
    string ziel;
    regex_t regex;
    // wird nur in Vorgaben gebraucht:
    zielmustercl(const char * const muster,const char * const ziel);
    zielmustercl(const char * const muster,const string& ziel);
    zielmustercl();
    int kompilier(const uchar obext=1);
    int setzemuster(const string& vmuster,const uchar obext=1);
    const string& holmuster() const;
    int obmusterleer() const;
}; // class zielmustercl

class urfxcl // urspruengliche Dateidaten vor Aufteilung an verschiedene Faxadressaten
{
 public:
    string teil;
    string ur;
    unsigned prio; // Prioritaet der Fax-Programme: 0 = capi und 0 = hyla per Konfigurationsdatei, 1= capi und 2= hyla per Faxdateiname
    urfxcl(const string& teil, const string& ur,unsigned prio): teil(teil), ur(ur), prio(prio) {}
};

class fxfcl // Faxfile
{
  public:
    string npdf; // nicht-PDF
    string spdf; // schon-PDF
    string ur;   // urspruenglicher Dateinamen
    unsigned prio; // Prioritaet der Fax-Programme: 0 = capi und 0 = hyla per Konfigurationsdatei, 1= capi und 2= hyla per Faxdateiname
		ulong pseiten; // PDF-Seitenzahl
    fxfcl(const string& npdf,const string& spdf,const string& ur,unsigned prio): npdf(npdf),spdf(spdf),ur(ur),prio(prio),pseiten(0) {}
    // nur fuer Initialisierung in fsfcl, Konstruktur /*1*/, nur fuer wegfaxen
    fxfcl(unsigned prio, const string& npdf,const string& spdf,ulong pseiten): npdf(npdf),spdf(spdf),prio(prio),pseiten(pseiten) {}
    fxfcl(const string& spdf,const string& ur,unsigned prio): npdf(""),spdf(spdf),prio(prio),pseiten(0) {}
    fxfcl() {}
};

class fsfcl : public fxfcl // Faxsendfile
{
  public:
    string id;   // id in spooltab
    string telnr;    // Telnr. des Adressaten
    string capisd; // capispooldatei
    int capids;  //capidials
    string hylanr; // hylanr
		string hqdt; // z.B. /var/spool/hylafax/doneq/q9902
    int hdialsn; // hyladials
		string hpages; // Seitenzahl
    uchar fobcapi; // ob es jetzt mit Capi weggefaxt werden muss
    uchar fobhyla; // ob es jetzt mit Hyla weggefaxt werden muss
    string adressat; // Name des Adressaten aus Faxdatei
		string idalt; // id in altspool
    string original; // base_name(spdf)
    string origvu;   // base_name(npdf)
    string idudoc;   // id des urspruenglichen Dateinamens in tabelle udoc
    string cspf;     // capispoolpfad
    string cdd;      // cdateidatum
    string cdials;   // capidials
    string ctries; // parameter aus capiprot
    string starttime; // parameter aus capiprot
    string dialstring; // parameter aus capiprot
    string hstate="0"; // Statuszahl ("state" in man sendq)
    string hstatus; // Textbeschreibung des letztes Fehlschlags
    string hstatuscode; // in xferfaxlog nicht gefunden
		time_t tts=0;
		time_t killtime=0;
		string number;   // Telefonnummer
    string hdials;   // hyladials
		string maxdials; // maxdials (hylafax)
    string hdd;      // hdateidatum
    string sendqgespfad; // kann fuer capi oder hyla verwendet werden
    string hgerg;  // hyla_gescheitert_erg
    int hversuzahl;
    FxStat capistat=init;// 1=wartend, 2=gesandt, 3=gescheitert, 4=fehlend (in spool keine Capi-Datei eingetragen oder die eingetragene gibts nicht)
    FxStat hylastat=init;// 1=wartend, 2=gesandt, 3=gescheitert, 4=fehlend (in spool keine hyla-Datei eingetragen oder die eingetragene gibts nicht)
  private:
  public:
		void archiviere(DB *const My, paramcl *const pmp, const struct stat *const entryp,const uchar obgescheitert, const FaxTyp ftyp, 
		                uchar *gel, const size_t aktc, const int obverb, const int oblog);
		int loeschecapi(const int obverb, const int oblog);
    int loeschehyla(paramcl *const pmp, const int obverb, const int oblog);
    /*1*/fsfcl(const string id, const string npdf, const string spdf, const string telnr, unsigned prio, const string capisd, int capids, 
		           const string hylanr, int hdialsn, uchar fobcapi, uchar fobhyla, const string adressat, ulong pseiten, string idalt):
         fxfcl(prio,npdf,spdf,pseiten), id(id), telnr(telnr), capisd(capisd), capids(capids), 
         hylanr(hylanr), hdialsn(hdialsn), fobcapi(fobcapi), fobhyla(fobhyla), adressat(adressat),idalt(idalt) {}
    /*2*/fsfcl(const string id,const string original): id(id), original(original) {}
    /*3*/fsfcl(const string id, const string capisd, const string hylanr, string const cspf): id(id), capisd(capisd), hylanr(hylanr), cspf(cspf) {}
    /*4*/fsfcl(const string& hylanr): hylanr(hylanr) {}
    /*5*/fsfcl(const string sendqgespfad, FxStat capistat): sendqgespfad(sendqgespfad), capistat(capistat) {}
		/*6*/fsfcl(const string& original, const string& origvu, uchar cnr): original(original), origvu(origvu) {}
    void setzcapistat(paramcl *pmp, struct stat *entrysendp);
    void capiausgeb(stringstream *ausgp, const string& maxctrials, uchar fuerlog=0, int obverb=0, int oblog=0,ulong faxord=0);
    void hylaausgeb(stringstream *ausgp, paramcl *pmp, int obsfehlt, uchar fuerlog=0, int obverb=0, uchar obzaehl=0, int oblog=0);
    int holcapiprot(int obverb);
		void scheitere(const string& wvz, const string& ngvz, const string& cuser, const string* const ziel=0, const int obverb=0, const int oblog=0);
}; // class fsfcl


class paramcl: public haupt // Programmparameter 
{
  private:
    svec modems;       // gefundene Modems
    uchar modemgeaendert=0; // hmodem neu gesetzt
//		long gmtoff; // Sekunden Abstand zur UTC-Zeit einschließlich Sommerzeit
  public:
		const string s1="mv -n ", s2="/2200/* ";
    schlArr hylcnfA; // fuer q1234 o.ae.
    uchar hgelesen=0; // Protokolldatei war auslesbar
    static constexpr const char *moeglhvz[2]={"/var/spool/fax","var/spool/hylafax"};
		string huser="uucp"; // "uucp" oder "fax"
    uchar obfcard=1;    // ob Fritzcard eingesteckt
    uchar obfcgeprueft=0; // ob schon geprueft, ob Fritzcard eingesteckt
    uchar obmodem=1;    // ob Modem angeschlossen
    uchar obmdgeprueft=0; // ob schon geprueft, ob Modem verfuegbar
    uchar obocrgeprueft=0; // ob ocrmypdf installiert ist
    const string spooltab="spool";
    const string altspool="altspool"; // Historie der Spooltabelle
    const string udoctab="udoc";
    uchar obvi=0;   // ob Konfigurationsdatei editiert werden soll
    uchar obvc=0;   // ob Capisuite-Konfigurationsdateien betrachtet werden sollen
		uchar obvh=0;   // ob Hylafax-Konfigurations- und Logdateindatei betrachtet werden sollen
		uchar obvs=0;   // ob Quelldateien bearbeitet werden sollen
    uchar loef=0;   // loesche eine Fax-Datei
    uchar loew=0;  // loeschewaise in der Datenbank, aber nicht mehr real nachweisbare Dateien in der Datenbank loeschen
    uchar loea=0; // loesche alle wartenden Faxe und zugehoerige Dateieintraege
    uchar loee=0; // empfangene Dateien loeschen, die nicht verarbeitet werden koennen
		uchar erneut=0;  // empfangenes Fax erneut bereitstellen
		uchar uml=0; // umleiten: vorzeitig den zweiten Weg aktivieren
    uchar kez=0;    // korrigiere Erfolgskennzeichen
    uchar bvz=0;    // bereinige Gescheitertenverzeichnis, letztes Gefaxtverzeichnis und Warteverzeichnis
    uchar anhl=0;    // <DPROG> anhalten
    uchar lista=0;   // liste Archiv auf
    uchar listf=0;   // liste gescheiterte auf
    uchar listi=0;   // liste Eingegangene auf
    uchar listw=0;   // liste wartende auf
    string suchstr;  // Wortteil, nach dem in alten Faxen gesucht werden soll
    string dszahl="30"; // Datensatzzahl fuer Tabellenausgaben
		size_t faxord=0; // Ordinalzahl des Faxes unter allen anstehenden Faxen
		ulong geszahl=0;
    ulong ankzahl=0; // Zahl der angekommenen Faxe
    ulong dbzahl=0; // Zahl der ueberprueften Datenbankeintraege
    ulong wzahl=0;
    ulong ezahl=0; // Zahl der Erfolgreichen
    ulong gzahl=0;
    ulong fzahl=0;
    ulong weizahl=0; // Zahl der weiteren wartenden Faxe, die nicht in der Spooltabelle dieses Programms eingetragen sind
    uchar gleichziel; // faxe auch ohne Fax-Erfolg auf Zielverzeichnis abspeichern
    uchar obocri; // empfangene Faxe OCR unterziehen
    uchar obocra; // gesandte Bilder OCR unterziehen
    uchar obcapi=1; // ob ueberhaupt die Capisuite verwendet werden soll, gesetzt in: pruefisdn(), lieskonfein(), rueckfragen(), getcommandline(), main()
    uchar obhyla=1; // ob ueberhaupt hylafax verwendet werden soll
    uchar konfobcapi; // ob obcapi in der Konfigurationsdatei eingestellt ist
    uchar konfobhyla; // ob obhyla in der Konfigurationsdatei eingestellt ist
    //    string hmodemstr; // Erkennung des Faxgeraetes nach /dev/tty, Standard ACM
    string hmodem;    // erkanntes und laufendes Modem ttyACM0
    //    string hmodname;  // ttyACM0
    uchar hylazuerst;  // ob ein Fax zuerst ueber Hylafax versucht werden soll zu faxen
    string dbq; // Datenbank
		string findvers; // find-Version (1=linux fund 2=intern mit readdir, 3=intern mit nftw 
		int ifindv; // integer-Variante der find-Version
		string cuser; // Linux-Benutzer fuer Capisuite, Samba
    string muser; // Benutzer fuer Mysql/MariaDB
    string mpwd;  // Passwort fuer Mysql/MariaDB
    DB* My=0;
		const size_t maxconz=12;//aktc: 0=pruefspool,pruefouttab,pruefudoc,pruefinctab,prueffuncgettel3,pruefstdfaxnr,aenderefax,rueckfragen 
		// bereinigevz,loeschewaise,loescheallewartenden,tu_lista,tu_listi,suchestr,Schluss, 1=korrigierecapi aus main, 
		//                          2=korrigierehyla aus main, 3=wegfaxen, untersuchespool, WVZinDatenbank, 4=zeigweitere, 5=empfarch,
		//                          6=faxemitC, 7=faxemitH, 9=korrigierecapi aus zeigweitere, 10=korrigierehyla aus zeigweitere,
    //													11=bereinigevz
    const string touta="outa"; // MariaDB-Tabelle fuer gesandte oder gescheiterte Faxe
    const string tudoc="udoc"; // MariaDB-Tabelle fuer gesandte oder gescheiterte Faxe
    const string tinca="inca"; // MariaDB-Tabelle fuer empfangene Faxe
		static const string initdhyladt; // /etc/init.d/hylafax
		uchar initdhyladt_gibts=0; // Datei initdhyladt existiert
    string cfaxconfdt; // /etc/capisuite/fax.conf oder /usr/local/etc/capisuite/fax.conf laut Handbuch
		string cfaxconfeigdt; // ~/autofax/cfaxconfdt
    string ccapiconfdt; // /etc/capisuite/capisuite.conf oder /usr/local/etc/capisuite/capisuite.conf laut Handbuch
    // Parameter aus /etc/capisuite/fax.conf:
    string spoolcapivz; // Verzeichnis der Capi-Spool-Dateien /var/spool/capisuite/
		string cempfavz; //  /var/spool/capisuite/" DPROG "arch/
    string cfaxuservz;    // /var/spool/capisuite/users/
    string cfaxusersqvz;    // /var/spool/capisuite/users/<user>/sendq
    string nextdatei;  // /var/spool/capisuite/users/<user>/sendq/fax-nextnr
    string cfaxuserrcvz;    // /var/spool/capisuite/users/<user>/received
		string cfaxuserrcfalschevz;  // /var/spool/capisuite/users/<user>/received/falsche
    string cdonevz; // Capisuite-Archiv: /var/spool/capisuite/done
    string cfailedvz; // Capisuite-Archiv der gescheiterten /var/spool/capisuite/failed
    string countrycode; // Landesvorwahl
    string citycode;    // Ortsvorwahl
    string msn;         // MSN fuer Fax
    string LongDistancePrefix; // Vorsatz fuer ausserorts
    string InternationalPrefix; // Vorsatz fuer ausserlandes
    string LocalIdentifier; // eigener Namen fuer Hylafax bis 10 Buchstaben
    string cFaxUeberschrift; // eigener Namen fuer Capisuite bis 20 Buchstaben
    string cklingelzahl; // Zahl der Klingeltoene, bis Capisuite einen Anruf annnimmt
    string hklingelzahl; // Zahl der Klingeltoene, bis Hylafax einen Anruf annnimmt
    string maxhdials;     // Zahl der Wahlversuche in Hylafax
		string maxcdials;    // Zahl der Wahlversuche in Capisuite
    uchar capizukonf=0; // capi zu konfigurieren
    uchar hylazukonf=0; // hyla zu konfigurieren
    string varsphylavz; // Verzeichnis der Hyla-Spool-Dateien /var/spool/hylafax oder /var/spool/fax
		string hempfavz;    // var/spool/(hyla)fax/" DPROG "arch
		string xferfaxlog; // varsphylavz + "/etc/xferfaxlog"; 
		string faxqpfad,hfaxdpfad; // /usr/local/sbin/faxq, /usr/local/sbin/hfaxq
		string faxgtpfad;   // /usr/lib/fax/faxgetty oder /usr/local/sbin/faxgetty
    string hsendqvz; // /var/spool/hylafax/sendq
    string hdoneqvz; // /var/spool/hylafax/doneq
    string harchivevz; // /var/spool/hylafax/archive
#ifdef _WIN32
    char cpt[255];
    DWORD dcpt;
#elif linux // _WIN32
    char cpt[MAXHOSTNAMELEN]; 
    size_t cptlen;
#endif // _WIN32 else
    string host="localhost";  // fuer MySQL/MariaDB

    string zufaxenvz;
    string wvz; // Warteverzeichnis
    string ngvz; // Nichtgefaxt-Verzeichnis (Gescheiterte)
    string empfvz; // Empfangsverzeichnis
    string maxcapiv; // maximale Versuchnr in Capi, bis Hyla versucht wird
    string maxhylav; // maixmale Versuchsnr in Hylafax, bis Capi versucht wird

    string sqlvz;  // Zahl der SQL-Befehle aus Vorgaben
    size_t sqlvzn=0; // Zahl der SQL-Befehle aus Vorgaben numerisch
    schlArr sqlVcnfA; // SQL-Array
    string sqlz;  // Zahl der SQL-Befehle
    size_t sqlzn=0; // Zahl der SQL-Befehle numerisch
    schlArr sqlcnfA; // SQL-Array

    zielmustercl *zmp; // Zielmusterzeiger
    schlArr zmcnfA; // dessen Serialisierung
    string zmz; // Zielmusterzahl
    size_t zmzn; // Zielmusterzahl numerisch
    zielmustercl *zmvp; // Zielmusterzeiger aus Vorgaben
    string zmvz; // Zielmusterzahl aus Vorgaben
    size_t zmvzn=0; // Zielmusterzahl numerisch aus Vorgaben

    schlArr cccnfA;  // capisuite.conf
    schlArr cfcnfA; // Capi-Konfiguration (fax.conf)
    string anfaxstr, ancfaxstr, anhfaxstr; // 'an Fax', "an cFax", "an hFax"
    string anstr; // ' an '
    string undstr;  //  'und'
		uchar keineverarbeitung=0; // wenn cronminuten geaendert werden sollen, vorher abkuerzen
    servc *sfaxq=0, *shfaxd=0, *shylafaxd=0, *sfaxgetty=0, *scapis=0;
    string modconfdt; // hylafax-Konfigurationsdatei, z.B. /var/spool/hylafax/etc/config.ttyACM0
    confdat *cfaxcdtp=0; // Zeiger auf ausgelesene /etc/capisuite/fax.conf
		string virtvz; //	instvz+"/ocrv";
	  string ocrmp; //	virtvz+"/bin/ocrmypdf";
		unsigned tage=0; // fuer korrigierecapi und korrigierehyla 
    svec vinca;
  private:
    void lgnzuw(); // in vorgaben, lieskonfein, getcommandl0, getcommandline, rueckfragen
    string neuerdateiname(const string& qpfad); // in wegfaxen
    void WVZinDatenbank(vector<fxfcl> *const fxvp, size_t aktc); // in wegfaxen
    string getzielvz(const string& datei); // in bereinigevz
    int setzegcp(const string& name, string *wert);
    void pruefcvz();
    void pruefsfftobmp();
		void instsfftobmp();
		void setzhylastat(fsfcl *fsf, uchar *hyla_uverz_nrp, uchar startvznr,int *obsfehltp=0, 
				struct stat *est=0);
		void konfcapi();
    int xferlog(fsfcl *fsfp/*, string *totpages=0, string *ntries=0, string *totdials=0, string *tottries=0, string *maxtries=0*/);
    void richtcapiher();
    void setzmodconfd();
    void setzzielmuster(confdat& afcd);
    void setzsql(confdat& afcd);
    void bereinigecapi(const size_t aktc);
	  int zupdf(const string* quell, const string& ziel, ulong *pseitenp=0, int obocr=1, int loeschen=1); // 0=Erfolg
		int holtif(const string& datei,ulong *seitenp=0,struct tm *tmp=0,struct stat *elogp=0,
		           string *absdrp=0,string *tsidp=0,string *calleridp=0,string *devnamep=0);
		void pruefmodcron();
		void pruefunpaper();
    int pruefocr();
		void unpaperfuercron(const string& ocrprog);
		void empfhyla(const string& ganz,size_t aktc, const uchar was,const string& nr=nix);
		void empfcapi(const string& stamm,size_t aktc, const uchar was,const string& nr=nix);
		void wandle(const string& udatei,const string& urname,const uchar iprio,svec& toktxt,svec& toknr,svec& tokname,
		            const string& anfxstr,const string& exten,vector<urfxcl> *urfxp);
		void inDbc(DB *My, const string& spooltab, const string& altspool, const string& spoolg, const fsfcl *const fsfp, 
		           const string& telnr, const size_t aktc);
		void faxemitC(DB *My, const string& spooltab, const string& altspool, fsfcl *fsfp, const string& ff);
		void inDBh(DB *My, const string& spooltab, const string& altspool, const string& hylaid, 
				const fsfcl *const fsfp,const string *tel, const size_t aktc);
		void faxemitH(DB *My, const string& spooltab, const string& altspool, fsfcl *fsfp, const string& ff);
		void hfaxsetup();
		int hconfig() const;
	public:
    paramcl(const int argc, const char *const *const argv);
    ~paramcl();
    void pruefggfmehrfach();
    void nextnum();
    string stdfaxnr(const string& faxnr);
    void getcommandl0();
    void pruefmodem();
		void setzfaxgtpfad();
		void pruefisdn();
    void liescapiconf();
    void VorgbAllg(); // allgemeine Vorgaben
		void MusterVorgb();
		void VorgbSpeziell() 
#ifdef DPROGcpp
			__attribute__((weak)) // implementationsspezifische Vorgaben (aber nur Quellcodeaenderung aenderbar, Modul vgb.cpp)
#endif // DPROGcpp
			;
		void lieskonfein();
		int  getcommandline();
		void rueckfragen();
		int setzhylavz(); // sucht das Hylaverzeichnis und setzt varsphylavz darauf, return 0, wenn nicht gefunden dann varsphylavz="", return 1
		void verzeichnisse();
		void pruefcron();
    void rufpruefsamba();
    int  initDB();
    int  pruefDB(const string& db);

    void bereinigevz(const size_t aktc/*=0*/);
		void dober(const string& wvz, set<string>& fdn,uchar aucherfolg,stringstream *ausgp,const size_t aktc,
				set<string> *cmisslp,set<string> *cgelup,set<string> *hmisslp,set<string> *hgelup);
		void anhalten();
		void getSender(const string& faxnr, string *getnamep, string *bsnamep,const size_t aktc);
    void tu_lista(const string& oberfolg,const string& submids=nix);
		void tu_listi(const uchar zurauswahl=0);
		void suchestr();
    int pruefsoffice(uchar mitloe=0);
    int pruefconvert();
    void clieskonf();
    void capisv();
    int pruefcapi();
    void hliesconf();
    int hconfigtty();
    int cservice();
    int hservice_faxq_hfaxd();
    void hylasv1();
    void hylasv2(hyinst hyinstart);
//    int hservice_faxgetty();
    int pruefhyla();
		int aenderefax(const int aktion=0,const size_t aktc=0); // aktion: 0=loeschen, 1=umleiten
		void empferneut();
    size_t  loeschewaise();
		size_t loescheallewartenden();
		void wegfaxen();
		void untersuchespool(uchar mitupd=1,const size_t aktc=3); // faxart 0=capi, 1=hyla 
		void bestimmtage();
    void zeigweitere();
    void sammlecapi(vector<fsfcl> *fsfvp,const size_t aktc);
    void sammlehyla(vector<fsfcl> *fsfvp,const size_t aktc);
    void korrigierecapi(const unsigned tage=90,const size_t aktc=1);
    void korrigierehyla(const unsigned tage=90,const size_t aktc=2);
    void empfarch(uchar obalte=0);
		void zeigueberschrift();
    void autofkonfschreib();
		void dovc();
		void dovh();
		void zeigdienste();
}; // class paramcl
