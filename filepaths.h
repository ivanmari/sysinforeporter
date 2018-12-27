#ifndef FILEPATHS_H
#define FILEPATHS_H

#include <vector>
#include <QString>
#include <QList>

const std::vector<std::pair<QString, bool>> paths {{"^.*oam-config\\.xml$", false},
                                  {"^.*/config/fmwconfig/oam-config\\.xml$", false},
                                  {"^.*adf-config\\.xml$", false},
                                  {"^.*/coherence./lib/coherence\\.jar$", false},
                                  {"^.*/coherence./lib/security/coherence-login\\.jar$", false},
                                  {"^.*tangosol-coherence.xml$", false},
                                  {"^.*tangosol-coherence-override\\.xml$", false},
                                  {"^.*/user_projects/domains/./config/config\\.xml$", true},
                                  {"^.*/server/lib/weblogic\\.jar$", false},
                                  {"^.*user_projects/domains/wl_server/startWebLogic.*$", false},
                                  {"^.*user_projects/domains/wl_server/bin/startWebLogic.*$", false},
                                  {"^.*/startWebLogic.*$", false},
                                  {"^.*startPIA\\.cmd$", false},
                                  {"^.*stopPIA\\.cmd$", false},
                                  {"^.*startPIA\\.sh$", false},
                                  {"^.*stopPIA\\.sh$", false},
                                  {"^.*/user_projects/domains/EPMSystem.*$", false},
                                  {"^.*/user_projects/domains/epmsystem.*$", false},
                                  {"^.*/servers/wls_forms/security/boot\\.properties$", false},
                                  {"^.*/servers/wls_reports/security/boot\\.properties$", false},
                                  {"^.*/servers/wls_forms/applications$", false},
                                  {"^.*/servers/wls_reports/applications$", false},
                                  {"^.*/formsweb.cfg$", false},
                                  {"^.*biee-domain.xml.*$", false},
                                  {"^.*/user_projects/domains/bifoundation_domain/bin/startWebLogic.*$", false},
                                  {"^.*nqsconfig.ini.*$", false},
                                  {"^.*oracle-bi-shared.jar.*$", false},
                                  {"^.*/bin/startessbase\\.sh$", false},
                                  {"^.*/bin/stopessbase\\.sh$", false},
                                  {"^.*/bin/startessbase\\.bat$", false},
                                  {"^.*/bin/stopessbase\\.bat$", false},
                                  {"^.*/essbaseserver/essbaseserver\\.setessbaseenv$", false},
                                  {"^.*/bin/oracle\\.exe$", false},
                                  {"^.*/bin/tnslsnr\\.exe$", false},
                                  {"^.*/bin/lsnrctl\\.exe$", false},
                                  {"^.*/bin/oracle$", false},
                                  {"^.*/bin/tnslsnr$", false},
                                  {"^.*/bin/lsnrctl$", false},
                                  {"^.*oes-common.jar.*$", false},
                                  {"^.*/oessm/oescommon/oes-common\\.jar$", false},
                                  {"^.*/oessm/./oes-common\\.jar$", false},
                                  {"^.*oes_client_home$", false},
                                  {"^.*ggsci.*$", false},
                                  {"^.*dirprm/.*\\.prm$", true}};


void getPatterns(const QString& username, const QString& basic_auth_hash, QList<QString>& patterns);

#endif // FILEPATHS_H
