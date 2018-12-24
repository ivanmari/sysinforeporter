#include "filepaths.h"
#include "restapi.h"

#include <QUrlQuery>

const char PATTERNS_ENDPOINT[] = "http://todo";
const QString USER = "user";
const QString PATTERNS_KEY = "patterns";

void getPatterns(const QString& username, const QString& basic_auth_hash, QList<QString>& patterns)
{
    QUrlQuery usr_query;

    usr_query.addQueryItem(USER, username);

    QUrl patterns_url(PATTERNS_ENDPOINT);

    patterns_url.setQuery(usr_query);

    RestApi rest(patterns_url, basic_auth_hash);

    QJsonObject json = rest.get();

    patterns = json.value(PATTERNS_KEY).toString().split(",");
}
