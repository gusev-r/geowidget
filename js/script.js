(function ($) {
    dialogSelectCity = $("<div/>");

    Drupal.ajax.prototype.commands.selectCity = function (ajax, response, status) {
        var parameters = {
            dialog: dialogSelectCity, 
            data: {city: response.arrParam}
        };
        var maybeyourcityWidget = new maybeyourcity(parameters);
        dialogSelectCity.html(maybeyourcityWidget.getApp());
        dialogSelectCity.dialog({autoOpen: false, modal: true, width: 'auto'});
        dialogSelectCity.dialog('open');
    }
}(jQuery));