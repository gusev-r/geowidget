(function ($) {
    maybeyourcity = function (parameters) {
        if (parameters === undefined){
            throw new Error('no parameters');
        }
        this.init(parameters);
    };
    
    maybeyourcity.prototype.init = function (parameters) {
        this.mainContainer = $('<div/>', {class: 'main-container'});
        this.treeroot = $('<div/>', {class: 'tree-root'});
        this.dialogSelectCity = parameters.dialog;
        this.header = $('<div/>', {'class': 'header'});
        this.title = $('<div/>', {class: 'title', text: ''});
        this.DefTree = $('<div/>', {class: 'tree'});
        this.data = parameters.data;
    }
    maybeyourcity.prototype.getApp = function () {
        this.creatHeader();
        this.treeroot.appendTo(this.mainContainer);
        var maybeyourcity = this;
        $.each(this.data.city, function () {
            maybeyourcity.createCity(this);
        });
        return this.mainContainer;
    };
    maybeyourcity.prototype.creatHeader = function () {
        var header = this.header;
        var defLetterBtn = $('<div/>', {class: 'letter'}).bind('click', $.proxy(this.clickAlphabet, this));
        var alphabet = ['а', 'б', 'в', 'г', 'д', 'е', 'ж', 'з', 'и', 'й', 'к', 'л', 'м', 'н', 'о', 'п', 'р', 'с', 'т', 'у', 'ф', 'х', 'ч', 'ш', 'щ', 'э', 'я'];
        $.each(alphabet, function () {
            header.append(defLetterBtn.clone(true).text(this[0]));
        });
        header.appendTo(this.mainContainer);
    };
    maybeyourcity.prototype.createCity = function (city) {
        var title = this.title
                .clone()
                .text(city.title)
                .attr('data-letter', city.title[0])
                .bind('click', $.proxy(this.clickCity, this));
        var element = $('<div/>', {
            'id': 'geo-city-id' + city.id,
            'class': 'city',
            'data-city-id': city.id
        }).append(title, this.DefTree.clone());
        if (city.parent_id === '0') {
            element.appendTo(this.treeroot);
        } else {
            var branch = this.treeroot[0].querySelector('#geo-city-id' + city.parent_id);
            if (branch) {
                branch.children[1].appendChild(element[0]);
            } else {
                element.appendTo(this.treeroot);
            }
        }
        ;
    };
    maybeyourcity.prototype.clickCity = function (event) {
        this.submitForm(event.target.parentElement.getAttribute("data-city-id"));
    };
    maybeyourcity.prototype.clickAlphabet = function (event) {
        this.header.removeClass('all');
        this.mainContainer.find('.title').removeClass('active');
        this.header.find('.letter').removeClass('active');

        $(event.target).val();
        var letter = $(event.target).text().toUpperCase();
        $(event.target).addClass('active');

        var city = this.mainContainer.find('[data-letter=' + letter + ']');
        city.addClass('active');
    };
    maybeyourcity.prototype.submitForm = function (id) {
        var city_id = $('#geo-form-wrapper  input[name=city_id]');
        city_id.val(id);
        this.dialogSelectCity.dialog('close');
        $('#geo-form-wrapper  .btn-set-city').mousedown();
    };

}(jQuery));