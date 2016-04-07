(function ($) {
    dialogSelectCity = $("<div/>");

    Drupal.ajax.prototype.commands.selectCity = function (ajax, response, status) {
        console.time('createCity');
        var $rootLevel = $('<div/>', {class: 'tree-root'});
        var GeoW = new geowidget($rootLevel, dialogSelectCity);
        GeoW.creatHeader();
        $.each(response.arrParam, function () {
            GeoW.createCity(this);
        });
        dialogSelectCity.html($rootLevel);
        dialogSelectCity.dialog({autoOpen: false, modal: true, width: 'auto'});
        dialogSelectCity.dialog('open');
        console.timeEnd('createCity');
    }
    function geowidget(mainContainer, dialogSelectCity) {
        this.mainContainer = mainContainer;
        this.dialogSelectCity = dialogSelectCity;
        this.header = {};
        this.title = $('<div/>', {class: 'title', text: ''});
        this.DefTree = $('<div/>', {class: 'tree'});
        this.creatHeader = function () {
            var header = this.header = $('<div/>', {'class': 'header'}).appendTo(this.mainContainer);
            var defLetterBtn = $('<div/>', {class: 'letter'}).bind('click', $.proxy(this.clickAlphabet, this));
            var alphabet = ['а', 'б', 'в', 'г', 'д', 'е', 'ж', 'з', 'и', 'й', 'к', 'л', 'м', 'н', 'о', 'п', 'р', 'с', 'т', 'у', 'ф', 'х', 'ч', 'ш', 'щ', 'э', 'я'];
            $.each(alphabet, function () {
                header.append(defLetterBtn.clone(true).text(this[0]));
            });
        };
        this.createCity = function (city) {
            var title = this.title.clone().text(city.title).attr('data-letter', city.title[0]).bind('click', $.proxy(this.clickCity, this));
            var element = $('<div/>', {
                'id': 'geo-city-id' + city.id,
                'class': 'city',
                'data-city-id': city.id
            }).append(title, this.DefTree.clone());
            if (city.parent_id === '0') {
                element.appendTo(this.mainContainer);
            } else {
                var branch = this.mainContainer[0].querySelector('#geo-city-id' + city.parent_id);
                if (branch) {
                    branch.children[1].appendChild(element[0]);
                } else {
                    element.appendTo(this.mainContainer);
                }
            }
            ;
        };
        this.clickCity = function (event) {
            this.submitForm(event.target.parentElement.getAttribute("data-city-id"));
        };
        this.clickAlphabet = function (event) {
            
            this.header.removeClass('all');
            this.mainContainer.find('.title').removeClass('active');
            this.header.find('.letter').removeClass('active');
            
            $(event.target).val();
            var letter = $(event.target).text().toUpperCase();
            $(event.target).addClass('active');

            var city = mainContainer.find('[data-letter='+letter+']');
            city.addClass('active');
        };
        this.submitForm = function (id) {
            var city_id = $('#geo-form-wrapper  input[name=city_id]');
            city_id.val(id);
            this.dialogSelectCity.dialog('close');
            $('#geo-form-wrapper  .btn-set-city').mousedown();
        };
    }
}(jQuery));